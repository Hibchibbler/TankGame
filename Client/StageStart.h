#ifndef StageStart_h_
#define StageStart_h_

#include "Common\GameStage.h"
//wann go to da sto wid me?
//want go to the store with me?

namespace tg
{
    class StageStart : public GameStage
    {
    public:
        StageStart();
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);

        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
    private:
    };
};



#endif

















