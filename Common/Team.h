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
            health = 100;
        }
        sf::Int32 health;
    private:
    };


    class Generator
    {
    public:
        Generator(){
            health = 100;
        }
        sf::Int32 health;
    private:
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
    };
};


#endif