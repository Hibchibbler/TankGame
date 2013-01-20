#include "GameClient.h"
#include <SFML/Window.hpp>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

int main()
{
    tg::GameClient gameClient(WINDOW_WIDTH,WINDOW_HEIGHT);
    bool done = false;

    gameClient.doInit();

    sf::Time frameTime;


    sf::sleep(sf::milliseconds(500));
    while (!done){

        gameClient.doRemoteEvents();

        gameClient.doLocalEvents();

        gameClient.doLoop();

        gameClient.doDraw(frameTime);

        sf::sleep(sf::milliseconds(0));
    }

    gameClient.doCleanup();
    return 0;
}

