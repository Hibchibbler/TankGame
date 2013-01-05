#include "StageStart.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include <SFGUI/SFGUI.hpp>

using namespace tg;

StageStart::StageStart(Game & g)
    : GameStage(g)
{

}

sf::Uint32 StageStart::doInit(Game & g)
{
    sfg::Table::Ptr table( sfg::Table::Create(  ) );

    sfg::Label::Ptr nameLabel;
    nameLabel = sfg::Label::Create("Name");    
    nameEntry = sfg::Entry::Create("Anon");
    table->Attach(nameLabel,sf::Rect<sf::Uint32>(0,0,1,1), 0);
    table->Attach(nameEntry,sf::Rect<sf::Uint32>(1,0,1,1), 3);

    sfg::Label::Ptr ipLabel;
    ipLabel = sfg::Label::Create("Address");    
    ipEntry = sfg::Entry::Create("192.168.1.9");
    table->Attach(ipLabel, sf::Rect<sf::Uint32>(0,1,1,1), 0);
    table->Attach(ipEntry, sf::Rect<sf::Uint32>(1,1,1,1), 3);
    
    sfg::Label::Ptr portLabel;
    portLabel = sfg::Label::Create("Port");
    portEntry = sfg::Entry::Create("8280");
    table->Attach(portLabel, sf::Rect<sf::Uint32>(0,2,1,1), 0);
    table->Attach(portEntry, sf::Rect<sf::Uint32>(1,2,1,1), 3);

    sfg::Button::Ptr joinButton;
    joinButton = sfg::Button::Create("Join");
    joinButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageStart::doJoin, this );
    table->Attach(joinButton, sf::Rect<sf::Uint32>(0,3,2,1), 3);


    sfg::Window::Ptr mywindow;
    mywindow = sfg::Window::Create();

    mywindow->SetTitle("Mega Blaster Client");
    mywindow->SetPosition(sf::Vector2f(100.0f,100.0f));
    mywindow->Add(table);

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
    e2.b = nameEntry->GetText().toAnsiString();
    setSummary(e2,1);

    Element e3;
    e3.b = ipEntry->GetText().toAnsiString();
    setSummary(e3,2);

    
    Element e4;
    e4.a = atoi(portEntry->GetText().toAnsiString().c_str());//"8280";
    setSummary(e4,3);

    desk.RemoveAll();
    ipEntry.reset();
    portEntry.reset();
    nameEntry.reset();
}

