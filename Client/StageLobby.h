#ifndef StageLobby_h_
#define StageLobby_h_

#include "Common\GameStage.h"

namespace tg
{
    class StageLobby : public GameStage
    {
    public:
        StageLobby();
        sf::Uint32 doRemoteEvent(TeamManager & teamMan, 
                                 ArenaManager & arenaMan,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);

        sf::Uint32 doLoop(Comm & comm, TeamManager & teamMan);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, TeamManager & teamMan);
    private:
    };
};



#endif

















