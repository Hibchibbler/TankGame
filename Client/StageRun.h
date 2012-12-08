#ifndef StageRun_h_
#define StageRun_h_

#include "Common\GameStage.h"
#include "Common\Player.h"
#include "Dashboard.h"
namespace tg
{
    class StageRun : public GameStage
    {
    public:
        StageRun();
        sf::Uint32 doInit();
        sf::Uint32 doRemoteEvent(TeamManager & teamMan, 
                                 ArenaManager & arenaMan,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);
        sf::Uint32 doWindowEvent(sf::RenderWindow & w, 
                                 sf::Event & event);
        sf::Uint32 doLoop(Comm & comm, TeamManager & teamMan);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, TeamManager & teamMan);
        sf::Uint32 doDraw(sf::RenderWindow & window, TeamManager & teamMan, ArenaManager & arenaMan, AssetManager & assetMan, sf::Time ft);
    private:
        Dashboard dash;
        Player thisPlayer;
        sf::View arenaView;
        sf::View dashView;

        sf::Uint32 scrWidth;
        sf::Uint32 scrHeight;

        sf::Uint32 attacking;
        sf::Vector2i curMousePos;
        sf::Vector2i lastMousePos;
        float curTurretAngle;
        float lastTurretAngle;
        sf::Clock stateOfPlayerClock;

        bool hasRxStateOfUnion;
        bool hasFocus;
        

    };
};


#endif

















