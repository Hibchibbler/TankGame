#include "GameClient.h"
#include <SFML/Window.hpp>

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

int main()
{
    int xres,yres;
    std::cout << "enter x resolution: ";
    std::cin >> xres;
    std::cout << "enter y resolution: ";
    std::cin >> yres;
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

