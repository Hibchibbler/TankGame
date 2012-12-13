#include "StageRun.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
using namespace tg;

#define PROJECTILE_DISTANCE 10

StageRun::StageRun()
    : GameStage()
{

}

sf::Uint32 StageRun::doRemoteEvent(TeamManager & teamMan, 
                                   ArenaManager & arenaMan,
                                   CommEvent & cevent,
                                   sf::Uint32 connId,
                                   sf::Uint32 msgId)
{
    Player &player = teamMan.getPlayer(connId);

    switch (msgId){
       
        case MsgId::WhoIs:
            std::cout << "Got OOB WhoIs" << std::endl;
            //If they want to know, then we'll tell em.
            player.state = PlayerState::SendingWhoIsAck;
            break;
        case MsgId::Id:{
            //They cannot reseat themselves in this stage.
            std::cout << "Got OOB Id" << std::endl;
            player.state = PlayerState::SendingIdNack;
            break;
        }case MsgId::StateOfPlayer:{
            //std::cout << "Got Action" << std::endl;
            //These are from players when they change the state of the game via an input mechanism
            //After crunching this event, send a Result packet to each player.

            //std::cout << << std::endl;
            sf::Uint32 playerAction;
            
            sf::Uint32 attacking = 0;
           //float temp1,temp2,temp3,temp4;

            cevent.packet >> player.tank.throttle;
            cevent.packet >> player.tank.bodyAngle;
            cevent.packet >> player.tank.turretAngle;
            //cevent.packet >> temp1;//player.tank.position.x;
            //cevent.packet >> temp2;//player.tank.position.y;
            //cevent.packet >> temp3;//player.tank.velocity.x;
            //cevent.packet >> temp4;//player.tank.velocity.y;
            cevent.packet >> attacking;

            if (attacking){
                std::cout << "#";
                player.state = PlayerState::EmitProjectile;
            }
            //update player 1 velocity based on updated throttle and body angle.
            /*player.tank.position.x =  player.tank.position.x + player.tank.throttle * (float)cos(player.tank.bodyAngle / (180/3.14156));
            player.tank.position.y =  player.tank.position.y + player.tank.throttle * (float)sin(player.tank.bodyAngle / (180/3.14156));*/
            player.tank.velocity.x = player.tank.throttle * (float)cos(player.tank.bodyAngle / (180/3.14156));
            player.tank.velocity.y = player.tank.throttle * (float)sin(player.tank.bodyAngle / (180/3.14156));
            //std::cout << player.tank.position.x << ", " << player.tank.position.y << std::endl;
            break;
        }
    }
    return 0;
}

bool isTankCollision(sf::Vector2f projPos, TeamManager & teamMan)
{
    for (int y = 1; y < 3 ; y++)
    {
        tg::Team::PlayerIterator tp = teamMan.getTeam(y).begin();
        for (;tp != teamMan.getTeam(y).end();tp++){
            //tp->tank.position
            if (tp->hasHost)
            {
                if (projPos.x > tp->tank.position.x &&
                    projPos.x < tp->tank.position.x+60 &&
                    projPos.y > tp->tank.position.y && 
                    projPos.y < tp->tank.position.y+30 )
                {
                    tp->tank.health--;
                    return true;
                }
            }
        }
    }

    return false;

    ////TODO: assumes size of tank.

    //    return true;
    //}
    //return false;
}

sf::Uint32 StageRun::doLoop(Comm & comm, TeamManager & teamMan)
{
    previousTime = currentTime;
    currentTime = velocityClock.restart();
    deltaTime = currentTime - previousTime;
    loopTime += deltaTime;
    
    //Player Update
    for (int y = 1; y < 3 ; y++)
    {
        tg::Team::PlayerIterator pi = teamMan.getTeam(y).begin();
        for (;pi != teamMan.getTeam(y).end();pi++){
        
            if (pi->hasHost == false)
                continue;
    
            switch (pi->state){
                case PlayerState::SendingWhoIsAck:
                    Messages::sendWhoIsAck(comm, teamMan, pi->connectionId);
                    pi->state = PlayerState::Running; 
                    break;
                case PlayerState::SendingIdNack:
                    Messages::sendIdNack(comm, teamMan, pi->connectionId);
                    pi->state = PlayerState::Running;
                    break;
                case PlayerState::Ready:
                    Messages::sendStart(comm, teamMan, pi->connectionId);
                    pi->state = PlayerState::Running;
                    break;
                case PlayerState::EmitProjectile:
                    
                    pi->prjctls.push_back(Projectile());
                    //TODO: THINK: do you want projectile to inherit the tanks' velocity?
                    
                    pi->prjctls.back().velocity.x = 30.0 * (float)cos(pi->tank.turretAngle/(180/3.14156));
                    pi->prjctls.back().velocity.y = 30.0 * (float)sin(pi->tank.turretAngle/(180/3.14156));
                    pi->prjctls.back().position.x = pi->tank.position.x + pi->prjctls.back().velocity.x*3;
                    pi->prjctls.back().position.y = pi->tank.position.y + pi->prjctls.back().velocity.y*3;

                    pi->prjctls.back().angle = pi->tank.turretAngle;
                    pi->state = PlayerState::Running;
                    break;
                case PlayerState::Running:
                     //Remove projectiles who have run out of power.
                    for (auto i = pi->prjctls.begin();i != pi->prjctls.end();)
                    {
                        if (i->totalDistance > PROJECTILE_DISTANCE*pi->tank.power*30)
                        {
                            i = pi->prjctls.erase(i);
                        }else{
                            i->position.x = i->position.x + i->velocity.x * loopTime.asSeconds()*20;
                            i->position.y = i->position.y + i->velocity.y * loopTime.asSeconds()*20;
                            i->totalDistance+=1;
                            bool yes = isTankCollision(i->position,teamMan);
                            if (yes)
                                i = pi->prjctls.erase(i);
                            else
                                i++;
                        }
                    }

                    //TODO: HACK: hardcoded arena boundaries based on debug map and a 125px wide floor tile.
                    if (pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*20 < (125) ||
                        pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*20 > (29*125) )
                    {
                        pi->tank.velocity.x = 0;
                    }

                    if (pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*20 < (125) ||
                        pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*20 > (29*125) )
                    {
                        pi->tank.velocity.y = 0;
                    }
                    pi->tank.position.x = pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*20;
                    pi->tank.position.y = pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*20;
            
                    break;
            }
        }
        //Game Update

    }
    

    if (updateStateTimer.getElapsedTime().asMilliseconds() > 75){
        //std::cout << "SS ";
        Messages::sendStateOfUnion(comm, teamMan);
        updateStateTimer.restart();
    }
    return getSummary(0);
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, TeamManager & teamMan)
{

    return 0;
}



