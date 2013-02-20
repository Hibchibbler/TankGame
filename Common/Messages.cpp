#include "Messages.h"

#include "Comm.h"
#include "TeamManager.h"
#include "ArenaManager.h"
//#include "Physics.h"
#include <iostream>

using namespace tg;


bool isVisible(sf::Vector2f posA, sf::Vector2f posB, float radius)
{
    float dx,dy;
    dx = posB.x - posA.x;
    dy = posB.y - posA.y;
    if (sqrt(dx*dx+dy*dy) < radius)
    {
        return true;
    }
    return false;
}
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


class Anaphlam
{
public:
    Anaphlam(int team, int slot, sf::Uint32 visible=1){
        t = team;
        s = slot;
        v = visible;
    }
    sf::Uint32 v;
    int t;
    int s;
    std::vector<sf::Vector2f> prj;
};

int Messages::sendStateOfUnion(Comm & comm, TeamManager & teamMan, ArenaManager & arenaMan)
{
    //std::cout << "Sent State" << std::endl;

    /*
    std::vector<tg::Connection>::iterator connection =  comm->Established.begin();
            for (;connection != comm->Established.end();){
    */
    for (int curTeam = 1;curTeam < 3;curTeam++)
    {
        int otherTeam = (curTeam == 1 ? 2 : 1);
        for (auto curSlot= 0;curSlot < teamMan.teams[curTeam].players.size();curSlot++)
        {
            //y = player we are sending state to.
            //t = team that player y belongs to.
            //otherTeam = team that player y does not belong to.


            if (teamMan.teams[curTeam].players[curSlot].hasHost){
                //We send state of union to curSlot
                //But, only what his base, minions, teammates, and hiself can see......

                tg::CommEvent event;
                

                event.connectionId = teamMan.teams[curTeam].players[curSlot].connectionId;

                event.packet << CommEventType::Data;
                event.packet << MsgId::StateOfUnion;

                for (int aTeam = 1;aTeam < 3;aTeam++){
                    std::vector<Anaphlam> visPlayers;
                    std::vector<Anaphlam> visCreep;
                    std::vector<Anaphlam> visDeathRay;
                    std::vector<Anaphlam> visHealRay;
                    //Tally visible players
                    for (auto aSlot= 0;aSlot < teamMan.teams[aTeam].players.size();aSlot++){
                        //Tally who is visible
                        if (teamMan.teams[aTeam].players[aSlot].hasHost){
                            sf::Uint32 visible = 0;
                            if (aTeam == curTeam)
                            {
                                visible = 1;
                            }

                            if (visible == 0 && isVisible(teamMan.teams[aTeam].players[aSlot].tank.position, 
                                                          teamMan.teams[curTeam].players[curSlot].tank.position, 1100) &&
                                                teamMan.teams[curTeam].players[curSlot].tank.health > 0)
                            {
                                visible = 1;
                            }

                            if (visible == 0){
                                for (auto c =0 ;c <teamMan.teams[curTeam].creep.size();c++)
                                {
                                    if (isVisible(teamMan.teams[aTeam].players[aSlot].tank.position,
                                                  teamMan.teams[curTeam].creep[c].position,550))
                                    {
                                        visible = 1;
                                        break;
                                    }
                                }
                            }

                            visPlayers.push_back(Anaphlam(aTeam, aSlot, visible));
                            for (auto p = 0;p <  teamMan.teams[aTeam].players[aSlot].prjctls.size();p++)
                            {
                                visible = 0;
                                if (aTeam == curTeam)
                                {
                                    visible = 1;
                                }
                                
                                if (visible == 0 && isVisible(teamMan.teams[aTeam].players[aSlot].prjctls[p].position, 
                                                              teamMan.teams[curTeam].players[curSlot].tank.position, 1100) &&
                                                    teamMan.teams[curTeam].players[curSlot].tank.health > 0)
                                {
                                    visible = 1;
                                }

                                if (visible == 0)
                                {
                                    for (auto c =0 ;c <teamMan.teams[curTeam].creep.size();c++)
                                    {
                                        if (isVisible(teamMan.teams[aTeam].players[aSlot].prjctls[p].position,
                                                      teamMan.teams[curTeam].creep[c].position,550))
                                        {
                                            visible = 1;
                                            break;
                                        }
                                    }
                                }

                                if (visible == 1)
                                    visPlayers.back().prj.push_back(teamMan.teams[aTeam].players[aSlot].prjctls[p].position);
                            }
                        }

                    }
                    //Transmit  visible Players and/or Projectiles
                    //Even if player is not visible, projectiles may be. So transmit them/
                    //but we still need to transmit enough information to indicate owner of the projectile

                    event.packet << visPlayers.size();
                    for (auto v = 0;v < visPlayers.size();v++){
                        Anaphlam a = visPlayers[v];
                        //teamMan.teams[a.t].players[a.s].
                        event.packet << teamMan.teams[a.t].players[a.s].slotNum;
                        event.packet << teamMan.teams[a.t].players[a.s].hasHost;
                        if (a.v==1)
                        {
                            event.packet << a.v;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.health;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.power;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.throttle;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.bodyAngle;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.turretAngle;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.position.x;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.position.y;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.velocity.x;
                            event.packet << teamMan.teams[a.t].players[a.s].tank.velocity.y;
                        }else{
                            event.packet << a.v;
                        }
                        event.packet << (sf::Uint32)visPlayers.back().prj.size();
                        for (int p = 0;p < visPlayers.back().prj.size();p++)
                        {
                            event.packet << visPlayers.back().prj[p].x;
                            event.packet << visPlayers.back().prj[p].y;
                        }
                    }


                    //Tally visible creep
                    for (auto y= 0;y < teamMan.teams[aTeam].creep.size();y++)
                    {
                        sf::Uint32 visible = 0;
                        if (aTeam == curTeam)
                        {
                            visible = 1;
                        }

                        if (visible == 0 && isVisible(teamMan.teams[aTeam].creep[y].position, 
                                                          teamMan.teams[curTeam].players[curSlot].tank.position, 1100) &&
                                                      teamMan.teams[curTeam].players[curSlot].tank.health > 0)    
                        {
                            visible = 1;
                        }

                        if (visible == 0)
                        {
                            for (auto c =0 ;c <teamMan.teams[curTeam].creep.size();c++)
                            {
                                if (isVisible(teamMan.teams[aTeam].creep[y].position,
                                              teamMan.teams[curTeam].creep[c].position,550))
                                {
                                    visible = 1;
                                    break;
                                }
                            }
                        }
                        if (visible == 1)
                            visCreep.push_back(Anaphlam(aTeam, y));
                    }

                    //Transmit visible Creep
                    event.packet << (sf::Uint32)visCreep.size();
                    for (auto v = 0;v < visCreep.size();v++){
                        Anaphlam a = visCreep[v];

                        event.packet << teamMan.teams[a.t].creep[a.s].position.x;
                        event.packet << teamMan.teams[a.t].creep[a.s].position.y;
                        event.packet << teamMan.teams[a.t].creep[a.s].angle;
                        event.packet << teamMan.teams[a.t].creep[a.s].creepType;
                    }

                    //Generator death Laser
                    for (int gi = 0;gi < arenaMan.getGeneratorCount(aTeam);gi++)
                    {
                        event.packet << teamMan.teams[aTeam].gen[gi].prjctls.size();
                        for (auto y= teamMan.teams[aTeam].gen[gi].prjctls.begin();y != teamMan.teams[aTeam].gen[gi].prjctls.end();y++)
                        {
                            event.packet << y->position.x;
                            event.packet << y->position.y;
                            event.packet << y->angle;
                        }
                    }

                    //Base heal Laser
                    event.packet << teamMan.teams[aTeam].base1.prjctls.size();
                    for (auto y= teamMan.teams[aTeam].base1.prjctls.begin();y != teamMan.teams[aTeam].base1.prjctls.end();y++)
                    {
                        event.packet << y->position.x;
                        event.packet << y->position.y;
                        event.packet << y->angle;
                    }
                    
                }

                //Explosions
                event.packet << teamMan.explosions.size();
                for (auto ex = teamMan.explosions.begin();ex != teamMan.explosions.end();ex++)
                {
                    event.packet << ex->position.x;
                    event.packet << ex->position.y;
                    event.packet << ex->type;
                }
   
                comm.Send(event);
            }
        }
    }
    //All players got explosions, clear the list..
    teamMan.explosions.clear();
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