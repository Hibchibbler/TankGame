#include "GameClient.h"
#include <SFML/Window.hpp>

int main()
{
    tg::GameClient gameClient;
    bool done = false;

    gameClient.doInit();

    sf::sleep(sf::milliseconds(500));
    while (!done){
        gameClient.doRemoteEvents();

        gameClient.doLocalEvents();

        gameClient.doLoop();

        sf::sleep(sf::milliseconds(30));
    }

    gameClient.doCleanup();
    return 0;
}

