////////////////WHERE I    AM    ///////////////////////////

//#include "Common\Messages.h"
//#include "Common\Comm.h"
//#include "Common\TeamManager.h"
//#include "Common\ArenaManager.h"
//#include "Common\LogFile.h"
#include "GameServer.h"
#include <SFML/System.hpp>
//#include <SFML/Window.hpp>
#include <iostream>

//enum ServerState{
//    Establishing,
//    Ready,
//    Running
//};
//
//sf::Clock stateTimer;
//ServerState serverState = Establishing;


//int doLocalInput(tg::Comm & comm, tg::TeamManager & teamMan);
//bool isIdValid(tg::TeamManager & teamMan, std::string name, sf::Uint32 team);
//int doLoop(tg::Comm & comm, tg::TeamManager & teamMan);
//int doCommEvents(tg::Comm & comm, tg::TeamManager & teamMan, tg::ArenaManager & arenaMan);
//int handleData(tg::Comm & comm, tg::TeamManager & teamMan, tg::ArenaManager & arenaMan, tg::Player & player, tg::CommEvent & event);

int main()
{
    tg::GameServer gameServer;
    bool done = false;

    gameServer.doInit();

    sf::sleep(sf::milliseconds(500));
    while (!done){
        gameServer.doRemoteEvents();

        gameServer.doLocalEvents();

        gameServer.doLoop();

        sf::sleep(sf::milliseconds(30));
    }

    gameServer.doCleanup();
    return 0;
}
//int doCommEvents(tg::Comm & comm, tg::TeamManager & teamMan, tg::ArenaManager & arenaMan)
//{
//    tg::CommEvent event;
//    while (comm.Receive(event)){
//        sf::Uint32 t;
//        event.packet >> t;
//        switch (t){
//            case tg::CommEvent::Acceptance:{
//                sf::Uint32 cid;
//                event.packet >> cid;
//                tg::Player p;
//                p.connectionId = cid;
//
//                teamMan.addPlayer(p,"New Guy", 0);
//
//                break;
//            }case tg::CommEvent::Disconnect:{
//                sf::Uint32 cid;
//                event.packet >> cid;
//                //remove player from either team0/1/2
//                teamMan.removePlayer(cid);
//                std::cout << "Player " << cid << " Disconnected" << std::endl;
//                break;
//            }case tg::CommEvent::Error:
//                break;
//            case tg::CommEvent::Data:{
//                
//                sf::Uint32 cid;
//                //std::cout << "Got Data" << std::endl;
//                cid = event.connectionId;
//
//                tg::Player & p = teamMan.getPlayer(cid);
//
//                
//                handleData(comm, teamMan, arenaMan,p,event);
//                break;
//            }case tg::CommEvent::Sent:{
//                break;
//            }
//        }
//    }
//
//    return 0;
//}
//int handleData(tg::Comm & comm, tg::TeamManager & teamMan, tg::ArenaManager & arenaMan, tg::Player & player, tg::CommEvent & event)
//{
//    sf::Uint32 mid;
//    event.packet >> mid;
//    switch (mid){
//        case tg::Ready:
//        {
//            std::cout << "Got Ready" << std::endl;
//            player.state = Ready;
//            //p = teamMan.getPlayer(cid);
//            /*tg::Team::PlayerIterator & pi1 = teamMan.teams[1].players.begin();
//            tg::Team::PlayerIterator & pi2 = teamMan.teams[2].players.begin();*/
//
//            break;
//        }
//        case tg::WhoIs:
//            std::cout << "Got WhoIs" << std::endl;
//            sendWhoIsAck(comm, teamMan, player.connectionId);
//            break;
//        case tg::Id:{
//                        
//            
//            //Received Id msg
//            std::string name;
//            sf::Uint32 team;
//            event.packet >> name;
//            event.packet >> team;
//            std::cout << "Got Id: " << name << ", " << team << std::endl;
//            
//            if (teamMan.isIdValid(name, team))
//            {
//                //the specified name and team are ok
//                //remove player form limbo
//                teamMan.removePlayer(player.connectionId);
//                            
//                //add player to requested team
//                player.tank.position = arenaMan.getStartPosition(team);
//                std::cout << "Adding " << name << " to team " << team << std::endl;
//                sf::Uint32 slot = teamMan.addPlayer(player, name, team);
//
//                //tell player they are established
//                sendIdAck(comm, teamMan, player.connectionId, slot);
//            }else{
//                sendIdNack(comm, teamMan, player.connectionId);
//            }
//            
//            break;
//        }case tg::Action:{
//            std::cout << "Got Action" << std::endl;
//            //These are from players when they change the state of the game via an input mechanism
//            //After crunching this event, send a Result packet to each player.
//
//
//            break;
//        }
//    }
//    return 0;
//}



//int doLocalInput(tg::Comm & comm, tg::TeamManager & teamMan)
//{
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)){
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
//            std::cout << "Starting" << std::endl;
//            serverState = Running;
//        }
//    }
//    return 0;
//}
//
//int doLoop(tg::Comm & comm, tg::TeamManager & teamMan)
//{
//    if (serverState == Establishing)
//    {
//    }
//    else if (serverState == Ready)
//    {
//    }
//    else if (serverState == Running)
//    {
//        if (stateTimer.getElapsedTime().asMilliseconds() > 500){
//            sendState(comm, teamMan);
//            stateTimer.restart();
//        }
//    }
//    return 0;
//}