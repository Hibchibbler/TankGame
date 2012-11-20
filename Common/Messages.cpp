#include "Messages.h"

#include "Comm.h"
#include "TeamManager.h"

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
    std::cout << "Sent StateOfPlayer" << std::endl;
    tg::CommEvent event;
    event.connectionId = cid;
    event.packet << CommEventType::Data;
    event.packet << MsgId::StateOfPlayer;
    event.packet << player.tank.throttle;
    event.packet << player.tank.bodyAngle;
    event.packet << player.tank.turretAngle;
    event.packet << player.tank.position.x;
    event.packet << player.tank.position.y;
    event.packet << player.tank.velocity.x;
    event.packet << player.tank.velocity.y;
    event.packet << attacking;// ?
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


int Messages::sendStateOfUnion(Comm & comm, TeamManager & teamMan)
{
    //std::cout << "Sent State" << std::endl;
    tg::CommEvent event;
    event.connectionId = -1;//Secret code word for "send to all"

    event.packet << CommEventType::Data;
    event.packet << MsgId::StateOfUnion;

    //Team 1 & 2 - not 0 because we don't advertise our limbo'ers
    for (int t = 1;t < 3;t++){
        event.packet << (sf::Uint32)teamMan.teams[t].players.size();
        for (auto y= teamMan.teams[t].players.begin();y != teamMan.teams[t].players.end();y++){
            event.packet << y->slotNum;
            event.packet << y->hasHost;
            event.packet << y->tank.bodyAngle;
            event.packet << y->tank.turretAngle;
            event.packet << y->tank.position.x;
            event.packet << y->tank.position.y;
            event.packet << y->tank.velocity.x;
            event.packet << y->tank.velocity.y;
        }
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
    return 0;
}