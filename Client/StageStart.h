#ifndef StageStart_h_
#define StageStart_h_

#include "Common\GameStage.h"
#include <SFGUI/SFGUI.hpp>

namespace tg
{
    class StageStart : public GameStage
    {
    public:
        StageStart();
        sf::Uint32 doInit(Game & g);
        sf::Uint32 doWindowEvent(sf::RenderWindow & window, sf::Event & event);
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);

        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
        sf::Uint32 doDraw(sf::RenderWindow &window,Game & g, sf::Time ft);
    private:
        void doJoin();
        sf::Clock deskUpdateClock;
        sfg::Desktop desk;
        sfg::Entry::Ptr ipEntry;
        sfg::Entry::Ptr portEntry;
    };
};



#endif

















