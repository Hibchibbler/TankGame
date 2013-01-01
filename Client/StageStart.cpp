#include "StageStart.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include <SFGUI/SFGUI.hpp>

using namespace tg;

StageStart::StageStart()
    : GameStage()
{

}

sf::Uint32 StageStart::doInit(Game & g)
{
    sfg::Box::Ptr box( sfg::Box::Create( sfg::Box::VERTICAL, 5.0f ) );

    sfg::Box::Ptr row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
    sfg::Label::Ptr ipLabel;
    ipLabel = sfg::Label::Create("Address");    
    row->Pack(ipLabel,true,true);   
    
    ipEntry = sfg::Entry::Create("192.168.1.9");
    row->Pack(ipEntry,true,true);
    box->Pack(row);


    row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
    sfg::Label::Ptr portLabel;
    portLabel = sfg::Label::Create("Port");    
    row->Pack(portLabel,true,true);   
    
    portEntry = sfg::Entry::Create("8280");
    row->Pack(portEntry,true,true);
    box->Pack(row);

    row = sfg::Box::Create( sfg::Box::HORIZONTAL, 5.0f );
    sfg::Button::Ptr joinButton;
    joinButton = sfg::Button::Create("Join");
    row->Pack(joinButton);
    box->Pack(row);

    joinButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageStart::doJoin, this );


    sfg::Window::Ptr mywindow;
    mywindow = sfg::Window::Create();

    mywindow->SetTitle("Mega Blaster Client");
    mywindow->Add(box);

    desk.Add(mywindow);
    return 0;
}
sf::Uint32 StageStart::doWindowEvent(sf::RenderWindow & window, sf::Event & event)
{
    desk.HandleEvent(event);
    return 0;
}

sf::Uint32 StageStart::doRemoteEvent(Game & g,
                                     CommEvent & cevent,
                                     sf::Uint32 connId,
                                     sf::Uint32 msgId)
{

    return 0;
}

sf::Uint32 StageStart::doLoop(Game & g)
{
    //setSummary(1,0);
    if (deskUpdateClock.getElapsedTime().asSeconds() >= 0.1f)
        desk.Update(deskUpdateClock.restart().asSeconds());
    return getSummary(0).a;
}
sf::Uint32 StageStart::doLocalInput(sf::RenderWindow & window, Game & g)
{
    return 0;
}

sf::Uint32 StageStart::doDraw(sf::RenderWindow &window,Game & g, sf::Time ft)
{
    window.resetGLStates();
    g.sfGui.Display(window);
    return 0;
}

void StageStart::doJoin()
{
    //This tells the Game that we are done
    Element e1;
    e1.a = 1;
    setSummary(e1,0);

    //And these are the summaries that the client will inspect.
    Element e2;
    e2.b = ipEntry->GetText().toAnsiString();
    setSummary(e2,1);

    
    Element e3;
    e2.a = atoi(portEntry->GetText().toAnsiString().c_str());//"8280";
    setSummary(e3,2);

    desk.RemoveAll();
    ipEntry.reset();
    portEntry.reset();
}