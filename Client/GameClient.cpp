#include "GameClient.h"

#include "Common\Messages.h"
#include "Common\LogFile.h"
#include "Common\Comm.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "Common\AssetManager.h"

using namespace tg;

GameClient::GameClient()
{
    //For Server, we want the Establish stage first.
    //
    state = GameClientState::Start;
    curStage = NULL;
    stageStart.setId(0);
    stageLobby.setId(1);
    stageRun.setId(2);

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
                sf::Uint32 mid;
                sf::Uint32 cid;

                cid = event.connectionId;

                tg::Player & p = teamMan.getPlayer(cid);
                event.packet >> mid;
                curStage->doRemoteEvent(teamMan, arenaMan, event, cid, mid);
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

        }
    }

    curStage->doLocalInput(window, teamMan);
 
    return 0;
}

sf::Uint32 GameClient::doInit()
{
    stageStart.doInit(600,600);
    stageLobby.doInit(600,600);
    stageRun.doInit(600,600);

    assetMan.load();
    arenaMan.load("Assets\\map1.txt");
    teamMan.load();
    //assetMan.doInit(...);
    window.create(sf::VideoMode(600,600,32),"Client");
    client.StartClient(8280,"127.0.0.1");//.StartServer(8280);
    

    return 0;
}

sf::Uint32 GameClient::doLoop()
{
    if (curStage != NULL)
    {
        sf::Uint32 summary = curStage->doLoop(client, teamMan);
        //Id is set in GameServer constructor.
        switch (curStage->getId()){
        case 0://stageStart
            switch (summary){
            case 0x1:
                std::cout << "Switching to StageLobby" << std::endl;
                curStage = &stageLobby;
                break;
            }
            break;
        case 1://stageLobby
            switch (summary){
            case 0x1:
                myTeam = curStage->getSummary(1);
                mySlot = curStage->getSummary(2);

                std::cout << myCID << ", " << myTeam << ", " << mySlot << std::endl;
                std::cout << "Switching to StageRun" << std::endl;
                curStage = &stageRun;
                curStage->setInput(600,0);
                curStage->setInput(600,0);
                curStage->setInput(myCID, 2);
                curStage->setInput(myTeam,3);
                curStage->setInput(mySlot,4);
                break;
            }
            break;
        case 2://stageRun
            switch (summary){
            case 0x1:
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

    curStage->doDraw(window, teamMan, assetMan, ft);

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



