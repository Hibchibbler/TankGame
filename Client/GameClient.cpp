#include "GameClient.h"

#include "Common\Messages.h"
#include "Common\LogFile.h"
#include "Common\Comm.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "Common\AssetManager.h"

using namespace tg;

GameClient::GameClient(sf::Uint32 w, sf::Uint32 h)
    : stageStart(*this),
      stageLobby(*this),
      stageRun(*this)
{
    //For Server, we want the Establish stage first.
    //
    state = GameClientState::Start;
    curStage = NULL;
    stageStart.setId(0);
    stageLobby.setId(1);
    stageRun.setId(2);

    scrWidth = w;
    scrHeight = h;

    myTeam = -1;
    myCID = -1;
    mySlot = -1;

    curStage = &stageStart;

}
GameClient::~GameClient()
{
    doCleanup();
}

sf::Uint32 GameClient::doRemoteEvents()
{
    if (curStage == NULL)
        return 1;
   //Do remote processing
    tg::CommEvent event;
    while (client.Receive(event)){
        sf::Uint32 t;
        event.packet >> t;
        switch (t){
            case CommEventType::Acceptance:{
                event.packet >> myCID;
                
                tg::Player p;
                p.state = PlayerState::SendingWhoIs;
                p.connectionId = myCID;
                
                teamMan.addPlayer(p, "Me", 0);//This entry will be properly populated during id negotiations
                
                //sendWhoIs(client, teamMan, cid);

                break;
            }case CommEventType::Disconnect:{
                sf::Uint32 cid;
                event.packet >> cid;
                //remove player from either team0/1/2
                std::cout << "Woah, Server Disconnected" <<std::endl;
                break;
            }case CommEventType::Error:
            
                sf::Uint32 msgId;
                event.packet >> msgId;
                std::cout << "Error!" << msgId << std::endl;
                break;
            case CommEventType::Data:{
                sf::Uint32 msgId;
                sf::Uint32 cid;

                cid = event.connectionId;

                //tg::Player & p = teamMan.getPlayer(cid);
                event.packet >> msgId;
                curStage->doRemoteEvent(*this, event, cid, msgId);
                break;
            }
        }
    }
    
    return 0;
}

sf::Uint32 GameClient::doLocalEvents()
{
    if (curStage == NULL)
        return 1;
    
    sf::Event wevent;
    while (window.pollEvent(wevent))
    {
        
        curStage->doWindowEvent(window, wevent);
        if (wevent.type == sf::Event::Resized)
        {
            scrWidth = wevent.size.width;
            scrHeight = wevent.size.height;
        }else if (wevent.type == sf::Event::Closed)
        {

        }
    }

    curStage->doLocalInput(window, *this);
 
    return 0;
}

sf::Uint32 GameClient::doInit()
{
    //Initialize Start Stage
    curStage = &stageStart;
    curStage->doInit(*this);

    //Initialize Lobby Stage
    curStage = &stageLobby;
    curStage->doInit(*this);

    //Initialize Run Stage
    curStage = &stageRun;
    curStage->doInit(*this);

    //Set the start stage..
    curStage = &stageStart;


    assetMan.load();
    arenaMan.load("Assets\\map5.txt");
    teamMan.load();
    //sf::VideoMode::getFullscreenModes().front().
    
    char fs;
    std::cout << "Fullscreen? (y or n):" ;
    std::cin >> fs;

    if (fs =='y')
        window.create(sf::VideoMode(scrWidth, scrHeight, 32), "Mega Blaster Tank Game", sf::Style::Fullscreen);
    else
        window.create(sf::VideoMode(scrWidth, scrHeight, 32), "Mega Blaster Tank Game");
    

    return 0;
}

sf::Uint32 GameClient::doLoop()
{

    if (curStage != NULL)
    {
        sf::Uint32 summary = curStage->doLoop(*this);
        //Id is set in GameServer constructor.
        switch (curStage->getId()){
        case 0://stageStart
            switch (summary){
            case 0x1:
                //Stage Done
                //Get summary details
                //transition to next stage;

                std::cout << "Start summary: " << stageStart.getSummary(1).b << std::endl;
                std::cout << "               " << stageStart.getSummary(2).b << std::endl;
                std::cout << "             @ " << stageStart.getSummary(3).a << std::endl;

                myName = stageStart.getSummary(1).b;
                myServerIp = stageStart.getSummary(2).b;
                myServerPort = stageStart.getSummary(3).a;

                client.StartClient(myServerPort, sf::IpAddress(myServerIp));

                std::cout << "Switching to StageLobby" << std::endl;
                curStage = &stageLobby;
                break;
            default:
                //stage still running
                break;
            }
            break;
        case 1://stageLobby
            switch (summary){
            case 0x1://Stage Done, transition to next stage;
                myTeam = curStage->getSummary(1).a;
                mySlot = curStage->getSummary(2).a;

                std::cout << myCID << ", " << myTeam << ", " << mySlot << std::endl;

                std::cout << "Switching to StageRun" << std::endl;
                curStage = &stageRun;
                
                break;
            default:
                //stage still running
                break;
            }
            break;
        case 2://stageRun
            switch (summary){
            case 0x1://Stage Done, transition to next stage;
                break;
            default:
                //stage still running
                break;
            }
            break;
        }
    }
     

    return 0;
}

sf::Uint32 GameClient::doCleanup()
{
    LogFile::get()->log(0,0,"GameClient::doCleanup");
    stageStart.doCleanup(*this);
    stageLobby.doCleanup(*this);
    stageRun.doCleanup(*this);
    return 0;
}

sf::Uint32 GameClient::doDraw(sf::Time ft)
{
    curStage->doDraw(window, *this, ft);
    return 0;
}

TeamManager & GameClient::getTeamMan()
{
    return teamMan;
}

Comm & GameClient::getComm()
{
    return client;
}



