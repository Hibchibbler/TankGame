#include "GameClient.h"
//#include "Common\Messages.h"
//#include "Common\Comm.h"
//#include "Common\TeamManager.h"
//#include "Common\AssetManager.h"
//
#include <SFML/Window.hpp>
//
//#include <iostream>
//using namespace tg;

int main()
{
    tg::GameClient gameClient;
    bool done = false;

    gameClient.doInit();

    sf::sleep(sf::milliseconds(500));
    while (!done){
        gameClient.doRemoteEvents();

        gameClient.doLocalEvents();

        gameClient.doLoop();

        sf::sleep(sf::milliseconds(30));
    }

    gameClient.doCleanup();
    return 0;
}


//
//enum ClientState{
//    Establishing,
//    Ready,
//    Running
//};
//
//std::string myName;
//sf::Uint32 myTeam;
//sf::Uint32 mySlot;
//sf::Uint32 myCID;
//bool hasFocus;
//ClientState clientState = Establishing;
//
//
//enum ActionDataType{
//    Control,
//    Attack
//};
//
//
//
//int doLocalInput(sf::RenderWindow & window, tg::Comm & comm, tg::TeamManager & teamMan)
//{
//    sf::Event winEvent;
//    while (window.pollEvent(winEvent)){
//        switch (winEvent.type)
//        {
//        case sf::Event::LostFocus:
//            hasFocus = false;
//            break;
//        case sf::Event::GainedFocus:
//            hasFocus = true;
//            break;
//        }
//    }
//
//    if (hasFocus)
//    {
//        bool control = false;
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
//            teamMan.teams[myTeam].players[mySlot].tank.throttle += 1;
//            if (teamMan.teams[myTeam].players[mySlot].tank.throttle > 25)
//                teamMan.teams[myTeam].players[mySlot].tank.throttle = 25;
//            control = true;
//        }
//    
//
//        //max speed lower in reverse
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
//            teamMan.teams[myTeam].players[mySlot].tank.throttle -= 1;
//            if (teamMan.teams[myTeam].players[mySlot].tank.throttle < -6)
//                teamMan.teams[myTeam].players[mySlot].tank.throttle = -6;
//            control = true;
//        }
//    
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
//            //Rotating left
//            teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = teamMan.teams[myTeam].players[mySlot].tank.bodyAngle -  5;
//            if (teamMan.teams[myTeam].players[mySlot].tank.bodyAngle >= 360.0f || teamMan.teams[myTeam].players[mySlot].tank.bodyAngle <= -360.0f ){
//                teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = 0;
//            }
//            control = true;
//        }
//    
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
//            //Rotating right
//            teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = teamMan.teams[myTeam].players[mySlot].tank.bodyAngle +  5;
//            if (teamMan.teams[myTeam].players[mySlot].tank.bodyAngle >= 360.0f || teamMan.teams[myTeam].players[mySlot].tank.bodyAngle <= -360.0f ){
//                teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = 0;
//            }
//            control = true;
//        }
//
//        bool attacking = false;
//
//        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
//            attacking = true;
//        }
//
//        if (control || attacking)
//            std::cout << (control ? "C" : "c") << (attacking ? "A" : "a") << std::endl;
//            tg::Messages::sendAction(comm,teamMan,teamMan.teams[myTeam].players[mySlot].connectionId, myTeam, mySlot, attacking);
//    }
//    return 0;
//}
//
//
//
//
//int doLoop(tg::Comm & comm, tg::TeamManager & teamMan)
//{
//
//    //Get myself onto a team...
//    //1. Have to see how many & who are on each team
//    //1.1 send WhoIs
//    //1.2 wait for WhoIsAck
//    //1.2.1 WhoIsAck contains everyone who is connected, and their team association.
//    //2. Tell server my name and team i'd like.
//    //3. The server will respond with an Ack or a Nack.
//    //3.1. if server Ack's, i move myself to my desired team.
//    //3.1.1 and move my Player object to the appropriate team container.
//    //3.2. if server Nack's, i change my name, or desired team. goto 2.
//    if (1)//clientState == Establishing)
//    {
//        //for (int y = 0;y < 3;y++){
//
//            tg::Team::PlayerIterator & pi = teamMan.teams[0].players.begin();        
//            for (;pi != teamMan.teams[0].players.end();pi++){
//        
//                if (pi->hasHost == false)
//                    continue;
//
//                switch (pi->state){
//                    case PlayerState::New:
//                        pi->state = PlayerState::SendingWhoIs;
//                        break;
//                    case PlayerState::SendingWhoIs:
//                        tg::Messages::sendWhoIs(comm, teamMan, pi->connectionId);
//                        pi->state = PlayerState::WaitingForWhoIsAck;
//                        break;
//                    case PlayerState::WaitingForWhoIsAck:
//                        std::cout << ".";//WaitingForWhoIsAck" << std::endl;
//                        break;
//                    case PlayerState::SendingId:
//                    {
//                        std::cout << "enter name:" << std::endl;
//                        std::cin >> myName;
//                        std::cout << "enter team:" << std::endl;
//                        std::cin >> myTeam;
//
//                        tg::Messages::sendId(comm, teamMan, pi->connectionId, myName, myTeam);
//                        pi->state = PlayerState::WaitingForIdAck;
//                        break;
//                    }
//                    case PlayerState::WaitingForIdAck:
//            
//                        std::cout << "," << std::endl;
//                        break;
//            
//                    case PlayerState::Ready:
//                        tg::Messages::sendReady(comm, teamMan, pi->connectionId);
//                        pi->state = PlayerState::WaitingForStart;
//                        break;
//                    case PlayerState::WaitingForStart:
//                        break;
//                    case PlayerState::SendingAction:
//                        break;
//                    case PlayerState::WaitingForState:
//                        break;
//                    case PlayerState::Running:
//                    {
//                        break;
//                    }
//                }
//            }
//        //}
//    }else if (clientState == Ready)
//    {
//
//    }else if (clientState == Running)
//    {
//
//    }
//    //Now do normal Game Processing on team1/2
//
//    /*pi = teamMan.getPlayerBegin(1);
//    for (;pi != teamMan.getPlayerEnd(1);pi++){
//
//    }
//
//    pi = teamMan.getPlayerBegin(2);
//    for (;pi != teamMan.getPlayerEnd(2);pi++){
//
//    }*/
//    return 0;
//}
//
//
//void printWhoIsAck(tg::CommEvent & event);
//
//int main()
//{
//    int me= -1;
//    bool done = false;
//    tg::Comm client;
//    tg::TeamManager teamMan;
//    sf::RenderWindow window(sf::VideoMode(500,500,32), "Tank Game");
//
//    
//    client.StartClient(8280, "127.0.0.1");
//    sf::sleep(sf::milliseconds(500));
//    while (!done){
//
//        ////Do remote processing
//        //tg::CommEvent event;
//        //while (client.Receive(event)){
//        //    sf::Uint32 t;
//        //    event.packet >> t;
//        //    switch (t){
//        //        case CommEventType::Acceptance:{
//        //            event.packet >> myCID;
//        //        
//        //            tg::Player p;
//        //            p.state = PlayerState::SendingWhoIs;
//        //            p.connectionId = myCID;
//        //        
//        //            teamMan.addPlayer(p, "Me", 0);//This entry will be properly populated during id negotiations
//        //        
//        //            //sendWhoIs(client, teamMan, cid);
//
//        //            break;
//        //        }case CommEventType::Disconnect:{
//        //            sf::Uint32 cid;
//        //            event.packet >> cid;
//        //            //remove player from either team0/1/2
//        //            std::cout << "Woah, Server Disconnected" <<std::endl;
//        //            break;
//        //        }case CommEventType::Error:
//        //            break;
//        //        case CommEventType::Data:{
//        //            sf::Uint32 mid;
//        //            sf::Uint32 cid;
//
//        //            cid = event.connectionId;
//
//        //            tg::Player & p = teamMan.getPlayer(cid);
//        //            event.packet >> mid;
//        //            switch (mid){
//        //            case MsgId::WhoIsAck:
//        //                {
//        //                    std::cout << "Got WhoIsAck" << std::endl;
//        //                    printWhoIsAck(event);
//        //                    
//        //                    //We only engage in establishment, when not already Established
//        //                    if (teamMan.teams[0].players[mySlot].state < PlayerState::Ready)
//        //                        teamMan.teams[0].players[mySlot].state = PlayerState::SendingId;
//        //                    
//        //                    break;
//        //                }
//        //                case MsgId::IdAck:
//        //                {
//        //                    event.packet >> mySlot;
//        //                    std::cout << "Got IdAck. We're slot " << mySlot <<   std::endl;
//        //                    p.state = PlayerState::Ready;
//        //                    break;
//        //                }
//        //                case MsgId::IdNack:
//        //                    std::cout << "Got IdNack" << std::endl;
//        //                    p.state = PlayerState::New;
//        //                    break;
//        //                case MsgId::State:
//        //                    std::cout << "Got State" << std::endl;
//        //                    break;
//        //                case MsgId::Start:
//        //                    std::cout << "Got Start" << std::endl;
//        //                    break;
//        //            }
//        //            break;
//        //        }
//        //    }
//        //}
//        doLocalInput(window,client, teamMan);
//        //Do local processing
//        doLoop(client, teamMan);
//        sf::sleep(sf::milliseconds(30));
//
//        window.clear();
//        window.display();
//    }
//
//    return 0;
//}
//
//
//
//void printWhoIsAck(tg::CommEvent & event)
//{
//    sf::Uint32 teamNum;
//    std::string name;
//
//    event.packet >> teamNum;
//    std::cout << "Team 1: " << std::endl;
//    for (sf::Uint32 i = 0;i < teamNum;i++){
//        event.packet >> name;
//        std::cout << "\t" << name << std::endl;
//    }
//
//    event.packet >> teamNum;
//    std::cout << "Team 2: " << std::endl;
//    for (sf::Uint32 i = 0;i < teamNum;i++){
//        event.packet >> name;
//        std::cout << "\t" << name << std::endl;
//    }
//
//}