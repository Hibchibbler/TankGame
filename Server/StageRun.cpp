#include "StageRun.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
using namespace tg;

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
           
            cevent.packet >> player.tank.throttle;
            cevent.packet >> player.tank.bodyAngle;
            cevent.packet >> player.tank.turretAngle;
            cevent.packet >> player.tank.position.x;
            cevent.packet >> player.tank.position.y;
            cevent.packet >> player.tank.velocity.x;
            cevent.packet >> player.tank.velocity.y;
            cevent.packet >> attacking;


            //update player 1 velocity based on updated throttle and body angle.
            player.tank.position.x =  player.tank.position.x + player.tank.throttle * (float)cos(player.tank.bodyAngle / (180/3.14156));
            player.tank.position.y =  player.tank.position.y + player.tank.throttle * (float)sin(player.tank.bodyAngle / (180/3.14156));
            std::cout << player.tank.position.x << ", " << player.tank.position.y << std::endl;
            break;
        }
    }
    return 0;
}

sf::Uint32 StageRun::doLoop(Comm & comm, TeamManager & teamMan)
{
    for (int y = 1; y < 3 ; y++)
    {
        tg::Team::PlayerIterator & pi = teamMan.getTeam(y).begin();
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
            }
        }
    }

    if (updateStateTimer.getElapsedTime().asMilliseconds() > 50){
        //std::cout << "Sent State" << std::endl;
        Messages::sendStateOfUnion(comm, teamMan);
        updateStateTimer.restart();
    }
    return getSummary(0);
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, TeamManager & teamMan)
{

    return 0;
}



