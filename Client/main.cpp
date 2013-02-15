#include "GameClient.h"
#include <SFML/Window.hpp>

#include <MMSystem.h>

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

int main()
{
    //std::cout << "sf::Texture::getMaximumSize() == " << sf::Texture::getMaximumSize() << std::endl;
    
    //std::cout << timeBeginPeriod(2) << std::endl;
    srand( time(NULL));
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
    }

    gameClient.doCleanup();
    //timeEndPeriod(2);
    return 0;
}

