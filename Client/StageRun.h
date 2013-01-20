#ifndef StageRun_h_
#define StageRun_h_

#include "Common\GameStage.h"
#include "Common\Player.h"
#include "Common\Images.h"
#include "Dashboard.h"
namespace tg
{
    class StageRun : public GameStage
    {
    public:
        StageRun(Game & g);
        sf::Uint32 doInit();
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);
        sf::Uint32 doWindowEvent(sf::RenderWindow & w, 
                                 sf::Event & event);
        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
        sf::Uint32 doDraw(sf::RenderWindow & window, Game & g, sf::Time ft);
    private:
        Dashboard dash;
        Player thisPlayer;
        sf::Vector2f targetPosition;

        //float arenaViewZoom;
        sf::View arenaView;
        sf::View dashView;
        float zoom;

        sf::Uint32 scrWidth;
        sf::Uint32 scrHeight;

        sf::Uint32 attacking;
        sf::Vector2i curMousePos;
        sf::Vector2i lastMousePos;
        float curTurretAngle;
        float lastTurretAngle;
        sf::Clock stateOfPlayerClock;
        sf::Clock updateStateClock;
        sf::Clock drawClock;
        sf::Clock loopClock;
        sf::Clock inputClock;
        sf::Clock accumulatingClock;

        sf::Time previousTime;
        sf::Time currentTime;
        sf::Time deltaTime;
        sf::Time frameTime;
        sf::Clock clock;

        std::vector<sf::Vector2f> posTrack;
        
        float timeCurUpdate;
        float timePrevUpdate;
        bool hasRxStateOfUnion;
        bool hasFocus;

        bool showShadow;

        std::list<Explosion> explosionsTankDeath;
        std::list<Explosion> explosionsTankHit;
        std::list<Explosion> explosionsCreepDeath;
        std::list<Explosion> explosionsCreepHit;
        std::list<Explosion> explosionsGeneratorDeath;
        std::list<Explosion> explosionsGeneratorHit;
        std::list<Explosion> explosionsBaseDeath;
        std::list<Explosion> explosionsBaseHit;

        std::list<Explosion> explosions;
    };
};


#endif

















