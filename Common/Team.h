#ifndef Team_h_
#define Team_h_

#include "Player.h"
#include "Creep.h"
#include <vector>


namespace tg
{
    class Base
    {
    public:
        Base()
        {
            health = 1000;
        }
        sf::Vector2f position;
        sf::Int32 health;
        std::vector<Projectile> prjctls;
    };


    class Generator
    {
    public:
        Generator(){
            health = 2500;
        }
        sf::Vector2f position;
        sf::Int32 health;
        std::vector<Projectile> prjctls;
    };


    class Team
    {
    public:
        Team(){
            players.reserve(5);
        }
        typedef std::vector<Player>::iterator PlayerIterator;
        std::vector<Player> players;
        std::vector<Creep> creep;
        Generator gen1;
        Base base1;
    };
};


#endif