#include "StageLobby.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"

using namespace tg;

StageLobby::StageLobby(Game & game)
    : GameStage(game)
{
    
}
sf::Uint32 StageLobby::doInit(Game & g)
{
    box =  sfg::Box::Create( sfg::Box::VERTICAL, 5.0f ) ;
    
    sfg::Box::Ptr row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
    row->Pack(sfg::Label::Create("Team 1"));
    row->Pack(sfg::Label::Create("Team 2"));
    box->Pack(row);

    for (int i = 0;i < 5;i++){
        row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
        team1[i] = sfg::Entry::Create("");    
        row->Pack(team1[i],true,true);   
        team2[i] = sfg::Entry::Create("");    
        row->Pack(team2[i],true,true);   
        box->Pack(row);
    }

    row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
    
    joinTeam1Button = sfg::Button::Create("Join Team 1");
    joinTeam1Button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageLobby::doJoinTeam1, this );
    row->Pack(joinTeam1Button);
    
    joinTeam2Button = sfg::Button::Create("Join Team 2");
    joinTeam2Button->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageLobby::doJoinTeam2, this );
    row->Pack(joinTeam2Button);
    box->Pack(row);

    //joinButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageStart::doJoin, this );

    mywindow = sfg::Window::Create();
    mywindow->SetTitle("Lobby");
    mywindow->SetPosition(sf::Vector2f(100.0f,100.0f));
    mywindow->Add(box);

    g.desk.Add(mywindow);
    return 0;
}

//sfg::Window::Ptr createNameDialog();
void StageLobby::doJoinTeam1()
{
    //Send Id
    
    tg::Team::PlayerIterator & pi = g.teamMan.teams[0].players.begin();        
    for (;pi != g.teamMan.teams[0].players.end();pi++){
        
        if (pi->hasHost == false)
            continue;
        pi->state = PlayerState::SendingId;
        g.myTeam = 1;
        break;
    }
    joinTeam1Button->Show(false);
    joinTeam2Button->Show(false);

    sfg::Box::Ptr row = sfg::Box::Create(sfg::Box::HORIZONTAL, 10.0f);
    sfg::Spinner::Ptr spinner = sfg::Spinner::Create();
    spinner->Start();
    row->Pack(spinner);

    sfg::Label::Ptr msg = sfg::Label::Create("Waiting for Host to start...");
    row->Pack(msg);
    box->Pack(row);
}

void StageLobby::doJoinTeam2()
{
    tg::Team::PlayerIterator & pi = g.teamMan.teams[0].players.begin();
    for (;pi != g.teamMan.teams[0].players.end();pi++){
        
        if (pi->hasHost == false)
            continue;
        pi->state = PlayerState::SendingId;
        g.myTeam = 2;
        break;
    }
    joinTeam1Button->Show(false);
    joinTeam2Button->Show(false);
    sfg::Box::Ptr row = sfg::Box::Create(sfg::Box::HORIZONTAL, 10.0f);
    sfg::Spinner::Ptr spinner = sfg::Spinner::Create();
    spinner->Start();
    row->Pack(spinner);

    sfg::Label::Ptr msg = sfg::Label::Create("Waiting for Host to start...");
    row->Pack(msg);
    box->Pack(row);
}


sf::Uint32 StageLobby::doWindowEvent(sf::RenderWindow & window, sf::Event & event)
{
    g.desk.HandleEvent(event);
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
            ////////////////////////////////////////
            sf::Uint32 teamSize;
            std::string name;
            cevent.packet >> teamSize;//Team 1
            //std::cout << "Team 1" << std::endl;
            for (int k = 0;k < teamSize;k++){
                cevent.packet >> name;
                team1[k]->SetText(name);
                //std::cout << name << ", ";
            }//std::cout << std::endl;

            cevent.packet >> teamSize;//Team 2
            //std::cout << "Team 2" << std::endl;
            for (int k = 0;k < teamSize;k++){
                cevent.packet >> name;
                team2[k]->SetText(name);
                //std::cout << name << ", ";
            }//std::cout << std::endl;

            //////////////////////////////////////////

          
            break;
        }
        case MsgId::IdAck:
        {
            sf::Uint32 mySlot;
            cevent.packet >> mySlot;
            
            //////////////////////
            Element e1;
            e1.a = mySlot;
            setSummary(e1,3);
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
            mywindow->Show(false);
            ///////////////////////
            //Tell game that this stage is finished.
            Element e0;
            e0.a = 1;
            setSummary(e0,0);

            //Tell game why this stage is finished - moving to run stage
            Element e1;
            e1.a = 0;
            setSummary(e1,1);
            ///////////////////////
            break;
    }
    return 0;
}



sf::Uint32 StageLobby::doLoop(Game & g)
{

    if (loopTime.getElapsedTime().asSeconds() > 0.100f)
    {
        g.desk.Update(loopTime.getElapsedTime().asSeconds());

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

        tg::Team::PlayerIterator & pi = g.teamMan.teams[0].players.begin();        
        for (;pi != g.teamMan.teams[0].players.end();pi++){
        
            if (pi->hasHost == false)
                continue;

            switch (pi->state){
                case PlayerState::New:
                    //tg::Messages::sendWhoIs(g.client,g.teamMan, pi->connectionId);
                    pi->state = PlayerState::SendingWhoIs;
                    break;
                case PlayerState::SendingWhoIs:
                    tg::Messages::sendWhoIs(g.client,g.teamMan, pi->connectionId);
                    pi->state = PlayerState::WaitingForWhoIsAck;
                    break;
                case PlayerState::WaitingForWhoIsAck:
                    //std::cout << ".";//WaitingForWhoIsAck" << std::endl;
                    break;
                case PlayerState::SendingId:
                {
                    Element e1;
                    e1.a = g.myTeam;
                    setSummary(e1,2);

                    tg::Messages::sendId(g.client, g.teamMan, pi->connectionId, g.myName, g.myTeam);
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
        loopTime.restart();
    }
    return getSummary(0).a;
}
sf::Uint32 StageLobby::doLocalInput(sf::RenderWindow & window, Game & g)
{
    return 0;
}


sf::Uint32 StageLobby::doDraw(sf::RenderWindow &window,Game & g, sf::Time ft)
{
    window.clear();
    window.resetGLStates();
    
    g.sfGui.Display(window);
    window.display();
    return 0;
}







