#include "Messages.h"

#include "Comm.h"
#include "TeamManager.h"
#include "ArenaManager.h"

#include <iostream>

using namespace tg;



int Messages::sendReady(Comm & comm, TeamManager & teamMan, int cid)
{
    std::cout << "Sent Ready" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::Ready;
    comm.Send(event);
    return 0;
}


int Messages::sendStateOfPlayer(Comm & comm, TeamManager & teamMan, int cid, int team, int slot,Player & player, sf::Uint32 attacking)
{
    //std::cout << "Sent StateOfPlayer" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::StateOfPlayer;

    event.packet << player.tank.turretAngle;
    event.packet << player.tank.accelerating;
    event.packet << player.tank.turningBody;
    event.packet << player.tank.attacking;
    
    comm.Send(event);
    return 0;
}

int Messages::sendMap(Comm & comm, std::string name, int cid)
{
    std::cout << "Sent Map" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::Map;
    event.packet << name;
    comm.Send(event);
    return 0;
}

int Messages::sendWhoIs(Comm & comm, TeamManager & teamMan, int cid)
{
    std::cout << "Sent WhoIs" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::WhoIs;
    comm.Send(event);
    return 0;
}
int Messages::sendId(Comm & comm,TeamManager & teamMan, int cid, std::string name, int team)
{
    std::cout << "Sent Id" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::Id;
    event.packet << name;
    event.packet << team;
    comm.Send(event);
    return 0;
}

int Messages::sendWhoIsAck(Comm & comm, TeamManager & teamMan, int cid)
{
    std::cout << "Sent WhoIsAck" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::WhoIsAck;
    //Team 1 & 2
    for (int t = 1;t < 3;t++){
        event.packet << (sf::Uint32)teamMan.teams[t].players.size();//.getTeamSize(1);
        for (auto y= teamMan.teams[t].players.begin();y != teamMan.teams[t].players.end();y++){
            event.packet << y->playerName;
        }
    }

    comm.Send(event);
    return 0;
}

int Messages::sendIdAck(Comm & comm, TeamManager & teamMan, int cid, sf::Uint32 slot)
{
    std::cout << "Sent IdAck" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::IdAck;
    event.packet << slot;
    comm.Send(event);
    return 0;
}
int Messages::sendIdNack(Comm & comm,TeamManager & teamMan, int cid)
{
    std::cout << "Sent IdNack" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::IdNack;
    comm.Send(event);
    return 0;
}


int Messages::sendStateOfUnion(Comm & comm, TeamManager & teamMan, ArenaManager & arenaMan)
{
    //std::cout << "Sent State" << std::endl;
    tg::CommEvent event;
    event.connectionId = -1;//Secret code word for "send to all"

    event.packet << CommEventType::Data;
    event.packet << MsgId::StateOfUnion;

    //Team 1 & 2 - not 0 because we don't advertise our limbo'ers
    int teamSize[3] =  { 0,0,0};

    for (int t = 0;t < 3;t++){
        for (auto y= teamMan.teams[t].players.begin();y != teamMan.teams[t].players.end();y++){
            if (y->hasHost)
                teamSize[t]++;
        }
    }

    for (int t = 1;t < 3;t++){
        event.packet << teamSize[t];
        for (auto y= teamMan.teams[t].players.begin();y != teamMan.teams[t].players.end();y++){
            if (y->hasHost){
                event.packet << y->slotNum;
                event.packet << y->hasHost;
                event.packet << y->tank.health;
                event.packet << y->tank.power;
                event.packet << y->tank.throttle;
                event.packet << y->tank.bodyAngle;
                event.packet << y->tank.turretAngle;
                event.packet << y->tank.position.x;
                event.packet << y->tank.position.y;
                event.packet << y->tank.velocity.x;
                event.packet << y->tank.velocity.y;
                event.packet << y->prjctls.size();
                for (int p = 0;p < y->prjctls.size();p++)
                {
                    event.packet << y->prjctls[p].position.x;
                    event.packet << y->prjctls[p].position.y;
                }
            }
        }
        //creep
        event.packet << teamMan.teams[t].creep.size();
        for (auto y= teamMan.teams[t].creep.begin();y != teamMan.teams[t].creep.end();y++)
        {
            event.packet << y->position.x;
            event.packet << y->position.y;
            event.packet << y->angle;
        }
        //Generator death Laser
        for (int gi = 0;gi < arenaMan.getGeneratorCount(t);gi++)
        {
            event.packet << teamMan.teams[t].gen[gi].prjctls.size();
            for (auto y= teamMan.teams[t].gen[gi].prjctls.begin();y != teamMan.teams[t].gen[gi].prjctls.end();y++)
            {
                event.packet << y->position.x;
                event.packet << y->position.y;
                event.packet << y->angle;
            }
        }

        //Base heal Laser
        event.packet << teamMan.teams[t].base1.prjctls.size();
        for (auto y= teamMan.teams[t].base1.prjctls.begin();y != teamMan.teams[t].base1.prjctls.end();y++)
        {
            event.packet << y->position.x;
            event.packet << y->position.y;
            event.packet << y->angle;
        }
    }

    //Explosions
    event.packet << teamMan.explosions.size();
    for (auto y = teamMan.explosions.begin();y != teamMan.explosions.end();)
    {
        event.packet << y->position.x;
        event.packet << y->position.y;
        event.packet << y->type;

        //Remove it from server, after it is sends it..
        y = teamMan.explosions.erase(y);
    }
   
    comm.Send(event);

    return 0;
}

int Messages::sendStart(Comm & comm, TeamManager & teamMan, int cid)
{
    std::cout << "Sent Start" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::Start;
    comm.Send(event);

    return 0;
}