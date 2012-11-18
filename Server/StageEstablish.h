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
        StageEstablish();
        sf::Uint32 doRemoteEvent(TeamManager & teamMan, 
                                 ArenaManager & arenaMan,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);

        sf::Uint32 doLoop(Comm & comm, TeamManager & teamMan);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, TeamManager & teamMan);
    private:
        SEState::STATE state;
    };

};

#endif





