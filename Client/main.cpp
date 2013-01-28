#include "GameClient.h"
#include <SFML/Window.hpp>

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

int main()
{
    tg::GameClient gameClient(640,480);
    bool done = false;

    gameClient.doInit();

    sf::Time frameTime;


    sf::sleep(sf::milliseconds(500));
    while (!done){
        sf::Uint32 ret;

        ret = gameClient.doRemoteEvents();

        ret = gameClient.doLocalEvents();
        if (ret){
            done = true;
            continue;
        }

        ret = gameClient.doLoop();
        if (ret){
            done = true;
            continue;
        }

        ret = gameClient.doDraw(frameTime);

        sf::sleep(sf::milliseconds(0));
    }

    gameClient.doCleanup();
    return 0;
}

