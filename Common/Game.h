#ifndef Game_h_
#define Game_h_


#include <SFML\Graphics.hpp>
#include <SFGUI\SFGUI.hpp>

#include "Comm.h"
#include "TeamManager.h"
#include "ArenaManager.h"
#include "AssetManager.h"

namespace tg
{
    

    class Game
    {
    public:
        virtual ~Game(){}
        Comm                    server;
        Comm                    client;

        TeamManager             teamMan;
        ArenaManager            arenaMan;
        AssetManager            assetMan;
        sf::RenderWindow        window;//don't know what's going to happen to this...
        
        sf::Uint32 myCID;
        sf::Uint32 mySlot;
        sf::Uint32 myTeam;
        std::string myName;
        std::string myServerIp;
        sf::Uint16 myServerPort;
        sf::Uint32 scrWidth;
        sf::Uint32 scrHeight;

        sfg::SFGUI sfGui;
    };

};


#endif











