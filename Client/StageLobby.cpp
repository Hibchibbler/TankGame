#include "StageLobby.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"

using namespace tg;

StageLobby::StageLobby()
    : GameStage()
{
}

sf::Uint32 StageLobby::doRemoteEvent(TeamManager & teamMan, 
                                     ArenaManager & arenaMan,
                                     CommEvent & cevent,
                                     sf::Uint32 connId,
                                     sf::Uint32 msgId)
{
    tg::Player & p = teamMan.getPlayer(connId);

    switch (msgId){
    case MsgId::WhoIsAck:
        {
            std::cout << "Got WhoIsAck" << std::endl;
            //printWhoIsAck(event);
                            
            p.state = PlayerState::SendingId;

            break;
        }
        case MsgId::IdAck:
        {
            sf::Uint32 mySlot;
            cevent.packet >> mySlot;
            
            //////////////////////
            setSummary(mySlot,2);
            ///////////////////////

            std::cout << "Got IdAck. We're slot " << mySlot <<   std::endl;
            p.state = PlayerState::Ready;
            break;
        }
        case MsgId::IdNack:
            std::cout << "Got IdNack" << std::endl;
            p.state = PlayerState::New;
            break;
        case MsgId::State:
            std::cout << "Got State" << std::endl;
            break;
        case MsgId::Start:
            std::cout << "Got Start" << std::endl;
            setSummary(1,0);
            break;
    }
    return 0;
}

sf::Uint32 StageLobby::doLoop(Comm & comm, TeamManager & teamMan)
{
    //    //Get myself onto a team...
    //1. Have to see how many & who are on each team
    //1.1 send WhoIs
    //1.2 wait for WhoIsAck
    //1.2.1 WhoIsAck contains everyone who is connected, and their team association.
    //2. Tell server my name and team i'd like.
    //3. The server will respond with an Ack or a Nack.
    //3.1. if server Ack's, i move myself to my desired team.
    //3.1.1 and move my Player object to the appropriate team container.
    //3.2. if server Nack's, i change my name, or desired team. goto 2.
   
        //for (int y = 0;y < 3;y++){

    tg::Team::PlayerIterator & pi = teamMan.teams[0].players.begin();        
    for (;pi != teamMan.teams[0].players.end();pi++){
        
        if (pi->hasHost == false)
            continue;

        switch (pi->state){
            case PlayerState::New:
                pi->state = PlayerState::SendingWhoIs;
                break;
            case PlayerState::SendingWhoIs:
                tg::Messages::sendWhoIs(comm, teamMan, pi->connectionId);
                pi->state = PlayerState::WaitingForWhoIsAck;
                break;
            case PlayerState::WaitingForWhoIsAck:
                std::cout << ".";//WaitingForWhoIsAck" << std::endl;
                break;
            case PlayerState::SendingId:
            {
                std::string myName;
                sf::Uint32 myTeam;

                std::cout << "enter name:" << std::endl;
                std::cin >> myName;
                std::cout << "enter team:" << std::endl;
                std::cin >> myTeam;

                ///////////////////////
                setSummary(myTeam,1);
                ///////////////////////
                
                tg::Messages::sendId(comm, teamMan, pi->connectionId, myName, myTeam);
                pi->state = PlayerState::WaitingForIdAck;
                break;
            }
            case PlayerState::WaitingForIdAck:
            
                std::cout << "," << std::endl;
                break;
            
            case PlayerState::Ready:
                tg::Messages::sendReady(comm, teamMan, pi->connectionId);
                pi->state = PlayerState::WaitingForStart;
                break;
           
        }
    }
        //}
    
    return getSummary(0);
}
sf::Uint32 StageLobby::doLocalInput(sf::RenderWindow & window, TeamManager & teamMan)
{
    return 0;
}










