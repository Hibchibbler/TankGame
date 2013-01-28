#ifndef StageStart_h_
#define StageStart_h_

#include "Common\GameStage.h"
#include <SFGUI/SFGUI.hpp>

namespace tg
{
    class StageStart : public GameStage
    {
    public:
        StageStart(Game & g);
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
        //void doApply();
        void doQuit();

        sf::Clock loopTime;

        sfg::Window::Ptr mywindow;//Kept to Show/Hide
        sfg::ComboBox::Ptr resolutionComboBox;//Kept to retrieve later
        sfg::Entry::Ptr ipEntry;//Kept to retrieve later
        sfg::Entry::Ptr portEntry;//Kept to retrieve later
        sfg::Entry::Ptr nameEntry;//Kept to retrieve later

        int resIndex;//store it so we can check if it changed.
    };
};



#endif

















