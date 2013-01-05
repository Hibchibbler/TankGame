#ifndef StageEstablish_h_
#define StageEstablish_h_

#include "Common\GameStage.h"

namespace tg
{
    //StageEstablishState Enum
    //
    struct SEState{enum STATE{
        WaitingForStart

    };};

    class GameServer;

    class StageEstablish : public GameStage
    {
    public:
        StageEstablish(Game & g);
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);

        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
    private:
        SEState::STATE state;
    };

};

#endif





