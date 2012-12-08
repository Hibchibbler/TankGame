#include "GameClient.h"
#include <SFML/Window.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

int main()
{
    tg::GameClient gameClient(WINDOW_WIDTH,WINDOW_HEIGHT);
    bool done = false;

    gameClient.doInit();

    sf::Time previousTime;
    sf::Time currentTime;
    sf::Time deltaTime;
    sf::Time frameTime;
    sf::Clock clock;

    sf::sleep(sf::milliseconds(500));
    while (!done){

        gameClient.doRemoteEvents();

        gameClient.doLocalEvents();

        gameClient.doLoop();

        previousTime = currentTime;
        currentTime = clock.restart();
        deltaTime = currentTime - previousTime;
        frameTime += deltaTime;
        
        gameClient.doDraw(frameTime);

        sf::sleep(sf::milliseconds(10));
    }

    gameClient.doCleanup();
    return 0;
}

