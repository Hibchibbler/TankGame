#ifndef GameServer_h_
#define GameServer_h_

#include "StageEstablish.h"
#include "StageRun.h"
#include "Common\Comm.h"
#include "Common\ArenaManager.h"
#include "Common\TeamManager.h"
#include <SFML\Graphics.hpp>

namespace tg
{
    struct GameServerState{
        enum GSS{
            Establish,
            Run
        };
    };

    /*struct Parameter{
        TeamManager * teamMan;
        ArenaManager * arenaMan;
        sf::RenderWindow * window;
    };*/

    class GameServer
    {
    public:
        GameServer();
        ~GameServer();

        sf::Uint32 doRemoteEvents();
        sf::Uint32 doLocalEvents();
        sf::Uint32 doInit();
        sf::Uint32 doLoop();
        sf::Uint32 doCleanup();
        sf::Uint32 doDraw();

        TeamManager & getTeamMan();
        Comm & getComm();
    private:
        Comm                    server;
        TeamManager             teamMan;
        ArenaManager            arenaMan;
        sf::RenderWindow        window;//don't know what's going to happen to this...

        StageEstablish          stageEst;
        StageRun                stageRun;
        GameServerState::GSS    state;
        GameStage *             curStage;
    private:

    };

};



#endif


