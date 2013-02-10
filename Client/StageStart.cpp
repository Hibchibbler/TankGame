#include "StageStart.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include <SFGUI/SFGUI.hpp>
#include <fstream>

using namespace tg;

StageStart::StageStart(Game & g)
    : GameStage(g)
{

}

sf::Uint32 StageStart::doInit(Game & g)
{
    std::string name;
    std::string address;
    std::string port;
    //int resIndex;

    //Load Start Menu settings from a simple text file
    std::ifstream fin("client_stage_start.txt");
    if (fin.is_open())
    {
        std::cout << "Read from client_stage_start.txt" << std::endl;
        fin >> name;
        fin >> address;
        fin >> port;
        //fin >> resIndex;
    }
    fin.close();

    //Construct Start Menu GUI
    sfg::Table::Ptr table( sfg::Table::Create(  ) );

    sfg::Label::Ptr nameLabel;    
    nameLabel = sfg::Label::Create("Name");
    nameEntry = sfg::Entry::Create(name);
    nameEntry->SetRequisition(sf::Vector2f(120.0f,0.0f));
    table->Attach(nameLabel,sf::Rect<sf::Uint32>(0,0,1,1), 0);
    table->Attach(nameEntry,sf::Rect<sf::Uint32>(1,0,1,1), 3);

    sfg::Label::Ptr ipLabel;
    ipLabel = sfg::Label::Create("Address");    
    ipEntry = sfg::Entry::Create(address);
    table->Attach(ipLabel, sf::Rect<sf::Uint32>(0,1,1,1), 0);
    table->Attach(ipEntry, sf::Rect<sf::Uint32>(1,1,1,1), 3);
    
    sfg::Label::Ptr portLabel;
    portLabel = sfg::Label::Create("Port");
    portEntry = sfg::Entry::Create(port);
    table->Attach(portLabel, sf::Rect<sf::Uint32>(0,2,1,1), 0);
    table->Attach(portEntry, sf::Rect<sf::Uint32>(1,2,1,1), 3);

    sfg::Label::Ptr resolutionLabel;
    sfg::Button::Ptr applyButton;
    sfg::Label::Ptr warningLabel;
    resolutionLabel = sfg::Label::Create("Resolution");
    resolutionComboBox = sfg::ComboBox::Create();
    warningLabel = sfg::Label::Create("Choose Wisely");
    warningLabel->SetLineWrap(true);
    applyButton = sfg::Button::Create("Apply");
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    
    //Populate video modes Combo box
    for (auto m = 0;m < modes.size();m++){
        if (modes[m].isValid())
        {
            std::stringstream ss;
            ss << modes[m].width << "x" << modes[m].height << " @ " << modes[m].bitsPerPixel<< " - " <<  (float)modes[m].width/(float)modes[m].height <<std::endl;
            if (modes[m].bitsPerPixel == 32)
                resolutionComboBox->AppendItem(sf::String(ss.str()));
            /*if (resIndex == m){
                resolutionComboBox->SelectItem(m);
            }*/
        }
    }
    table->Attach(resolutionLabel, sf::Rect<sf::Uint32>(0,3,1,1), 0);
    table->Attach(resolutionComboBox,sf::Rect<sf::Uint32>(1,3,1,1), 3);
    table->Attach(warningLabel,sf::Rect<sf::Uint32>(3,3,1,1), 3);

    sfg::Button::Ptr joinButton;
    joinButton = sfg::Button::Create("Join");
    joinButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageStart::doJoin, this );
    table->Attach(joinButton, sf::Rect<sf::Uint32>(0,4,2,1), 3);

   /* sfg::Button::Ptr quitButton;
    quitButton = sfg::Button::Create("Quit");
    quitButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageStart::doQuit, this );
    table->Attach(quitButton, sf::Rect<sf::Uint32>(0,5,2,1), 3);*/


    mywindow = sfg::Window::Create();
    
    mywindow->SetTitle("Mega Blaster Client");
    mywindow->SetRequisition(sf::Vector2f(425,300));
    mywindow->SetPosition(sf::Vector2f(100.0f,100.0f));
    mywindow->Add(table);
    
    g.desk.Add(mywindow);

    return 0;
}
sf::Uint32 StageStart::doWindowEvent(sf::RenderWindow & window, sf::Event & event)
{
    g.desk.HandleEvent(event);
   
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
    if (loopTime.getElapsedTime().asSeconds() > 0.050f)
    {
        g.desk.Update(loopTime.getElapsedTime().asSeconds());

        loopTime.restart();
    }
    return getSummary(0).a;
}
sf::Uint32 StageStart::doLocalInput(sf::RenderWindow & window, Game & g)
{
    return 0;
}

sf::Uint32 StageStart::doDraw(sf::RenderWindow &window,Game & g, sf::Time ft)
{
    window.clear();
    window.resetGLStates();
    g.sfGui.Display(window);
    window.display();
    return 0;
}

void StageStart::doQuit()
{
    //Tell Game that this stage is finished
    Element e0;
    e0.a = 1;
    setSummary(e0,0);

    //Tell game why this stage is finished - we quit
    Element e1;
    e1.a = 1;
    setSummary(e1,1);
}


void StageStart::doJoin()
{
    //Save start menu settings to a simple text file.
    std::ofstream fout("client_stage_start.txt", std::ios_base::out);
    if (fout.is_open())
    {
        std::cout << "Wrote to client_stage_start.txt" << std::endl;
        fout << nameEntry->GetText().toAnsiString() << std::endl;
        fout << ipEntry->GetText().toAnsiString() << std::endl;
        fout << portEntry->GetText().toAnsiString() << std::endl;
    }
    fout.close();
    sfg::ComboBox::IndexType it = resolutionComboBox->GetSelectedItem();
    if (it != sfg::ComboBox::NONE)
    {
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            g.window.close();
            sf::ContextSettings cs;
            cs.antialiasingLevel = 0;
            std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
            g.window.create(modes[it], "Mega Blaster Tank Game", sf::Style::Default,cs);//, sf::Style::Fullscreen);
        }
        //Tell Game that this stage is finished
        Element e0;
        e0.a = 1;
        setSummary(e0,0);

        //Tell game why this stage is finished - moving to lobby stage
        Element e1;
        e1.a = 0;
        setSummary(e1,1);

        //And these are the summaries that the client will expect.
        Element e2;
        e2.b = nameEntry->GetText().toAnsiString();
        setSummary(e2,2);

        Element e3;
        e3.b = ipEntry->GetText().toAnsiString();
        setSummary(e3,3);
    
        Element e4;
        e4.a = atoi(portEntry->GetText().toAnsiString().c_str());//"8280";
        setSummary(e4,4);

        mywindow->Show(false);
 
    }
}

