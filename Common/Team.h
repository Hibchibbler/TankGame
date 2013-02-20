#ifndef Team_h_
#define Team_h_

#include "Player.h"
#include "Creep.h"
#include <vector>
#include <list>


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
            lastFiringAngle=0;
        }
        sf::Vector2f position;
        sf::Int32 health;
        std::vector<Projectile> prjctls;
        float lastFiringAngle;
    };

    //TODO: max of 5 generators.
    //Add more, and you will have a cryptic crash.
    class Team
    {
    public:
        Team(){
            lastUID=0;
            players.reserve(5);
            gen.push_back(Generator());
            gen.push_back(Generator());
            gen.push_back(Generator());
            gen.push_back(Generator());
            gen.push_back(Generator());
        }
        typedef std::vector<Player>::iterator PlayerIterator;
        std::vector<Player> players;
        std::vector<Creep> creep;
        std::vector<Generator> gen;

        Base base1;

        sf::Uint32 lastUID;
    };
};


#endif