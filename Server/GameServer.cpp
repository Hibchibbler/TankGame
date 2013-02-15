#include "GameServer.h"
#include "Common\LogFile.h"
#include "Common\Comm.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"

using namespace tg;

GameServer::GameServer()
    : stageEst(*this),
      stageRun(*this)
{
    //For Server, we want the Establish stage first.
    //
    numConnects=0;
    state = GameServerState::Establish;
    curStage = NULL;
    stageEst.setId(0);
    stageRun.setId(1);

    curStage = &stageEst;
}
GameServer::~GameServer()
{
    doCleanup();
}

sf::Uint32 GameServer::doRemoteEvents()
{
    if (curStage == NULL)
        return 1;
    CommEvent event;
    while (server.Receive(event)){
        sf::Uint32 t;
        event.packet >> t;
        switch (t){
            case CommEventType::Acceptance:{
                LogFile::get()->log(0,0,"Acceptance");
                sf::Uint32 cid;
                event.packet >> cid;
                Player p;
                p.connectionId = cid;

                teamMan.addPlayer(p,"New Guy", 0);
                numConnects++;
                break;
            }case CommEventType::Disconnect:{
                LogFile::get()->log(0,0,"Disconnect");
                sf::Uint32 cid;
                event.packet >> cid;
                //remove player from either team0/1/2
                teamMan.removePlayer(cid);
                std::cout << "Player " << cid << " Disconnected" << std::endl;
                numConnects--;
                if (numConnects==0)
                {
                    curStage = &stageEst;
                    state = GameServerState::Establish;
                }
                break;
            }case CommEventType::Error:
            LogFile::get()->log(0,0,"Error");
                break;
            case CommEventType::Data:{
                //LogFile::get()->log(0,0,"Data");
                sf::Uint32 msgId;
                sf::Uint32 connId;
                //std::cout << "Got Data" << std::endl;
                connId = event.connectionId;

                event.packet >> msgId;
                curStage->doRemoteEvent(*this, event, connId, msgId);
                break;
            }case CommEventType::Sent:{
                break;
            }
        }
    }
    
    return 0;
}

sf::Uint32 GameServer::doLocalEvents()
{
    if (curStage == NULL)
        return 1;
    
    sf::Event wevent;
    while (window.pollEvent(wevent))
    {
        curStage->doWindowEvent(window, wevent);
    }

    curStage->doLocalInput(window, *this);
 
    return 0;
}

sf::Uint32 GameServer::doInit()
{
    arenaMan.load("Assets\\small.map");
    assetMan.load();
    teamMan.load();
    
    stageEst.doInit(*this);
    stageRun.doInit(*this);
    
    server.StartServer(8280);

    return 0;
}

sf::Uint32 GameServer::doLoop()
{
    if (curStage != NULL)
    {
        sf::Uint32 summary = curStage->doLoop(*this);
        //Id is set in GameServer constructor.
        switch (curStage->getId()){
        case 0://stageEst
            switch (summary){
            case 0x1:
                state = GameServerState::Run;
                std::cout << "Switching to StageRun" << std::endl;
                curStage = &stageRun;
                break;
            }
            break;
        case 1://stageRun
            break;
        }
    }

    return 0;
}

sf::Uint32 GameServer::doCleanup()
{
    LogFile::get()->log(0,0,"GameServer::doCleanup");
    stageEst.doCleanup(*this);
    stageRun.doCleanup(*this);
    return 0;
}



TeamManager & GameServer::getTeamMan()
{
    return teamMan;
}

Comm & GameServer::getComm()
{
    return server;
}



