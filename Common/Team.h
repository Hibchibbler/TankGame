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
            health = 2000;
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
            gen.push_back(Generator());
            gen.push_back(Generator());
        }
        typedef std::vector<Player>::iterator PlayerIterator;
        std::vector<Player> players;
        std::vector<Creep> creep;
        std::vector<Generator> gen;

        Base base1;
    };
};


#endif