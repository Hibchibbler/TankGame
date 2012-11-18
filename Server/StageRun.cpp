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
        }case MsgId::Action:{
            std::cout << "Got Action" << std::endl;
            //These are from players when they change the state of the game via an input mechanism
            //After crunching this event, send a Result packet to each player.

            //std::cout << << std::endl;
            sf::Uint32 playerAction;
            
            float turretAngle;
            cevent.packet >> playerAction;
            cevent.packet >> turretAngle;

            switch (playerAction)
            {
            case PlayerAction::Attack:
                break;
            case PlayerAction::BodyRight:
                break;
            case PlayerAction::BodyLeft:
                break;
            case PlayerAction::ThrottleUp:
               break;
            case PlayerAction::ThrottleDown:
                break;
            case PlayerAction::TurretMove:
                break;
            }
            //teamMan.getPlayer(connId).

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
    return getSummary(0);
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, TeamManager & teamMan)
{

    return 0;
}



