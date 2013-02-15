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
            window.close();
            return 1;
        }
    }

    curStage->doLocalInput(window, *this);
 
    return 0;
}

sf::Uint32 GameClient::doInit()
{
    //Set the start stage..
    curStage = &stageStart;
    curStage->doInit(*this);

    assetMan.load();
    arenaMan.load("Assets\\small.map");
    teamMan.load();


    sf::ContextSettings cs = window.getSettings();

    std::cout << "depthBits=" << cs.depthBits << std::endl;
    std::cout << "stencilBits=" << cs.stencilBits << std::endl;
    std::cout << "antialiasingLevel=" << cs.antialiasingLevel << std::endl;
    std::cout << "majorVersion=" << cs.majorVersion << std::endl;
    std::cout << "minVersion=" << cs.minorVersion << std::endl;

    window.create(sf::VideoMode(800,600,32), "Mega Blaster Tank Game - Resolution Choosing Mode");
    
    window.setVerticalSyncEnabled(true);
    
    //window.setFramerateLimit(30);
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
                if (stageStart.getSummary(1).a == 0){

                    std::cout << "Start summary: " << stageStart.getSummary(2).b << std::endl;
                    std::cout << "               " << stageStart.getSummary(3).b << std::endl;
                    std::cout << "             @ " << stageStart.getSummary(4).a << std::endl;

                    myName = stageStart.getSummary(2).b;
                    myServerIp = stageStart.getSummary(3).b;
                    myServerPort = stageStart.getSummary(4).a;

                    client.StartClient(myServerPort, sf::IpAddress(myServerIp));

                    std::cout << "Switching to StageLobby" << std::endl;
                    curStage = &stageLobby;
                    curStage->doInit(curStage->getOwner());

                }else if (stageStart.getSummary(1).a == 1){
                    std::cout << "Quitting" << std::endl;
                    window.close();
                    client.Stop();
                    return 1;
                }
                break;
            default:
                //stage still running
                break;
            }
            break;
        case 1://stageLobby
            switch (summary){
            case 0x1://Stage Done, transition to next stage;
                if (stageLobby.getSummary(1).a == 0){
                    myTeam = curStage->getSummary(2).a;
                    mySlot = curStage->getSummary(3).a;

                    std::cout << myCID << ", " << myTeam << ", " << mySlot << std::endl;

                    std::cout << "Switching to StageRun" << std::endl;
                    curStage = &stageRun;
                    curStage->doInit(curStage->getOwner());
                }else if (stageLobby.getSummary(1).a == 1){
                    std::cout << "Quitting" << std::endl;
                    window.close();
                    client.Stop();
                    return 1;
                }
                break;
            default:
                //stage still running
                break;
            }
            break;
        case 2://stageRun
            switch (summary){
            case 0x1://Stage Done, transition to next stage;
                if (stageRun.getSummary(1).a == 0){

                }else if (stageRun.getSummary(1).a == 1){
                    std::cout << "Quitting" << std::endl;
                    window.close();
                    client.Stop();
                    return 1;
                }

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



