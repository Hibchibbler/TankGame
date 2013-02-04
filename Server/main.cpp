#include "GameServer.h"
#include <SFML/System.hpp>

#include <iostream>

int main()
{

    tg::GameServer gameServer;
    bool done = false;

    gameServer.doInit();

    sf::sleep(sf::milliseconds(500));
    while (!done){
        gameServer.doRemoteEvents();

        gameServer.doLocalEvents();

        gameServer.doLoop();

        //sf::sleep(sf::milliseconds(0));
    }

    gameServer.doCleanup();
    return 0;
}





