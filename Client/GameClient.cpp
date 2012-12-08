#include "GameClient.h"

#include "Common\Messages.h"
#include "Common\LogFile.h"
#include "Common\Comm.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "Common\AssetManager.h"

using namespace tg;

GameClient::GameClient(sf::Uint32 w, sf::Uint32 h)
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
                break;
            case CommEventType::Data:{
                sf::Uint32 msgId;
                sf::Uint32 cid;

                cid = event.connectionId;

                //tg::Player & p = teamMan.getPlayer(cid);
                event.packet >> msgId;
                curStage->doRemoteEvent(teamMan, arenaMan, event, cid, msgId);
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
        }
    }

    curStage->doLocalInput(window, teamMan);
 
    return 0;
}

sf::Uint32 GameClient::doInit()
{
    //Initialize Start Stage
    curStage = &stageStart;
    curStage->setInput(scrWidth,    0);
    curStage->setInput(scrHeight,   1);
    curStage->doInit();

    //Initialize Lobby Stage
    curStage = &stageLobby;
    curStage->setInput(scrWidth,    0);
    curStage->setInput(scrHeight,   1);
    curStage->doInit();

    //Initialize Run Stage
    curStage = &stageRun;
    curStage->setInput(scrWidth,    0);
    curStage->setInput(scrHeight,   1);
    curStage->setInput(myCID,       2);
    curStage->setInput(myTeam,      3);
    curStage->setInput(mySlot,      4);
    curStage->doInit();

    //Set the start stage..
    curStage = &stageStart;


    assetMan.load();
    arenaMan.load("Assets\\map1.txt");
    teamMan.load();
    
    window.create(sf::VideoMode(scrWidth, scrHeight,32),"Client");
    client.StartClient(8280,"127.0.0.1");//.StartServer(8280);

    return 0;
}

sf::Uint32 GameClient::doLoop()
{
    if (curStage != NULL)
    {
        curStage->setInput(scrWidth,    0);
        curStage->setInput(scrHeight,   1);
        curStage->setInput(myCID,       2);
        curStage->setInput(myTeam,      3);
        curStage->setInput(mySlot,      4);

        sf::Uint32 summary = curStage->doLoop(client, teamMan);
        //Id is set in GameServer constructor.
        switch (curStage->getId()){
        case 0://stageStart
            switch (summary){
            case 0x1:
                //Stage Done
                //Get summary details
                //transition to next stage;
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
                myTeam = curStage->getSummary(1);
                mySlot = curStage->getSummary(2);

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
    stageStart.doCleanup();
    stageLobby.doCleanup();
    stageRun.doCleanup();
    return 0;
}

sf::Uint32 GameClient::doDraw(sf::Time ft)
{
    window.clear();

    curStage->doDraw(window, teamMan, arenaMan, assetMan, ft);

    window.display();
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



