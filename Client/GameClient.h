#ifndef GameClient_h_
#define GameClient_h_

#include "StageStart.h"
#include "StageLobby.h"
#include "StageRun.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\ArenaManager.h"
#include "Common\TeamManager.h"
#include "Common\AssetManager.h"
#include <SFML/Graphics.hpp>

#include <SFGUI/SFGUI.hpp>

namespace tg
{
    struct GameClientState{
        enum GCS{
            Start,
            Lobby,
            Run
        };
    };
    class GameClient : public Game
    {
    public:
        GameClient(sf::Uint32 w, sf::Uint32 h);
        ~GameClient();

        sf::Uint32 doRemoteEvents();
        sf::Uint32 doLocalEvents();
        sf::Uint32 doInit();
        sf::Uint32 doLoop();
        sf::Uint32 doCleanup();
        sf::Uint32 doDraw(sf::Time ft);

        TeamManager & getTeamMan();
        Comm & getComm();
    private:
        //Comm client;
        //TeamManager teamMan;
        //ArenaManager arenaMan;
        //AssetManager assetMan;
        //sf::RenderWindow window;

        StageStart stageStart;
        StageLobby stageLobby;
        StageRun   stageRun;
        sf::Clock drawClock;
        sf::Clock loopClock;
        //sf::Uint32 myCID;
        //sf::Uint32 mySlot;
        //sf::Uint32 myTeam;

        //sf::Uint32 scrWidth;
        //sf::Uint32 scrHeight;

        GameClientState::GCS state;
        GameStage * curStage;


    };

};



#endif


