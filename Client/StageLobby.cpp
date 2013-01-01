#include "StageLobby.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"

using namespace tg;

StageLobby::StageLobby()
    : GameStage()
{
}
sf::Uint32 StageLobby::doInit(Game & g)
{
    sfg::Box::Ptr box( sfg::Box::Create( sfg::Box::VERTICAL, 5.0f ) );
    
    for (int i = 0;i < 5;i++){
        sfg::Box::Ptr row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
        team1[i] = sfg::Entry::Create("");    
        row->Pack(team1[i],true,true);   
        team2[i] = sfg::Entry::Create("");    
        row->Pack(team2[i],true,true);   
        box->Pack(row);
    }

    sfg::Box::Ptr row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
    sfg::Button::Ptr joinTeam1Button;
    joinTeam1Button = sfg::Button::Create("Join Team 1");
    row->Pack(joinTeam1Button);
    sfg::Button::Ptr joinTeam2Button;
    joinTeam2Button = sfg::Button::Create("Join Team 2");
    row->Pack(joinTeam2Button);
    box->Pack(row);

    //joinButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageStart::doJoin, this );


    sfg::Window::Ptr mywindow;
    mywindow = sfg::Window::Create();

    mywindow->SetTitle("Lobby");
    mywindow->Add(box);

    desk.Add(mywindow);
    return 0;
}


sf::Uint32 StageLobby::doWindowEvent(sf::RenderWindow & window, sf::Event & event)
{
    desk.HandleEvent(event);
    return 0;
}

sf::Uint32 StageLobby::doRemoteEvent(Game & g,
                                     CommEvent & cevent,
                                     sf::Uint32 connId,
                                     sf::Uint32 msgId)
{
    tg::Player & p = g.teamMan.getPlayer(connId);

    switch (msgId){
    case MsgId::WhoIsAck:
        {
            std::cout << "Got WhoIsAck" << std::endl;
            //printWhoIsAck(event);
            ////////////////////////////////////////
            sf::Uint32 teamSize;
            std::string name;
            cevent.packet >> teamSize;//Team 1
            std::cout << "Team 1" << std::endl;
            for (int k = 0;k < teamSize;k++){
                cevent.packet >> name;
                std::cout << name << ", ";
            }std::cout << std::endl;

            cevent.packet >> teamSize;//Team 2
            std::cout << "Team 2" << std::endl;
            for (int k = 0;k < teamSize;k++){
                cevent.packet >> name;
                std::cout << name << ", ";
            }std::cout << std::endl;

            ////////////////////////////////////////

            if (p.state == PlayerState::WaitingForWhoIsAck){
                p.state = PlayerState::SendingId;
            }

            break;
        }
        case MsgId::IdAck:
        {
            sf::Uint32 mySlot;
            cevent.packet >> mySlot;
            
            //////////////////////
            Element e1;
            e1.a = mySlot;
            setSummary(e1,2);
            ///////////////////////

            std::cout << "Got IdAck. We're slot " << mySlot <<   std::endl;
            p.state = PlayerState::Ready;
            break;
        }
        case MsgId::IdNack:
            std::cout << "Got IdNack" << std::endl;
            p.state = PlayerState::New;
            break;
        case MsgId::Start:
            std::cout << "Got Start" << std::endl;
            ///////////////////////
            Element e1;
            e1.a = 1;
            setSummary(e1,0);
            ///////////////////////
            break;
    }
    return 0;
}



sf::Uint32 StageLobby::doLoop(Game & g)
{
    if (deskUpdateClock.getElapsedTime().asSeconds() >= 0.1f)
        desk.Update(deskUpdateClock.restart().asSeconds());
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

    tg::Team::PlayerIterator & pi = g.teamMan.teams[0].players.begin();        
    for (;pi != g.teamMan.teams[0].players.end();pi++){
        
        if (pi->hasHost == false)
            continue;

        switch (pi->state){
            case PlayerState::New:
                pi->state = PlayerState::SendingWhoIs;
                break;
            case PlayerState::SendingWhoIs:
                tg::Messages::sendWhoIs(g.client,g.teamMan, pi->connectionId);
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
                Element e1;
                e1.a = myTeam;
                setSummary(e1,1);
                ///////////////////////
                
                tg::Messages::sendId(g.client, g.teamMan, pi->connectionId, myName, myTeam);
                pi->state = PlayerState::WaitingForIdAck;
                break;
            }
            case PlayerState::WaitingForIdAck:
            
                std::cout << "," << std::endl;
                break;
            
            case PlayerState::Ready:
                tg::Messages::sendReady(g.client, g.teamMan, pi->connectionId);
                pi->state = PlayerState::WaitingForStart;
                break;
        }
    }

    return getSummary(0).a;
}
sf::Uint32 StageLobby::doLocalInput(sf::RenderWindow & window, Game & g)
{
    return 0;
}


sf::Uint32 StageLobby::doDraw(sf::RenderWindow &window,Game & g, sf::Time ft)
{
    window.resetGLStates();
    g.sfGui.Display(window);
    return 0;
}







