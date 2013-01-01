#ifndef StageRun_h_
#define StageRun_h_

#include "Common\GameStage.h"

namespace tg
{
    class StageRun : public GameStage
    {
    public:
        StageRun();
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);

        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
    private:
        sf::Clock updateStateTimer;
        sf::Clock sendStateTimer;
        sf::Clock velocityClock;
        sf::Clock minionAddClock;
        sf::Clock accumulatingClock;

        bool stateUpdated;

        
        sf::Time previousTime;
        sf::Time currentTime;
        sf::Time deltaTime;
        sf::Time loopTime;
    };

};

#endif





