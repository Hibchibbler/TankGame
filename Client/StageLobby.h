#ifndef StageLobby_h_
#define StageLobby_h_

#include "Common\GameStage.h"
#include <SFGUI\SFGUI.hpp>

namespace tg
{
    //This stage steers the local player to connecting to the server,
    //and manages the state associated with that.
    //It uses team 0, and assumes only the local player
    //will be found in team 0.
    class StageLobby : public GameStage
    {
    public:
        StageLobby(Game & g);
        sf::Uint32 doInit(Game & g);
        sf::Uint32 doWindowEvent(sf::RenderWindow & window, sf::Event & event);
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId
                                 );

        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
        sf::Uint32 doDraw(sf::RenderWindow &window,Game & g, sf::Time ft);
    
        void doJoinTeam1();
        void doJoinTeam2();
        void doNameOk();

        struct LobbyState{
            enum{
                
            };
        };
  private:
        sf::Clock whoIsPollClock;
        sf::Clock loopTime;

        sfg::Window::Ptr    nameWindow;
        sfg::Window::Ptr    createNameDialog();
        sf::Clock           oopTime;
        
        sfg::Window::Ptr    mywindow;//Kept to Show/Hide
        sfg::Box::Ptr       box;//kept to add the spinner later.
        sfg::Button::Ptr    joinTeam1Button;//kept to hide later.
        sfg::Button::Ptr    joinTeam2Button;//kept to hide later.
        
        sf::Uint32          myTeam;
        sfg::Entry::Ptr     yourName;
        sfg::Entry::Ptr     team1[5];
        sfg::Entry::Ptr     team2[5];
    };
};



#endif


