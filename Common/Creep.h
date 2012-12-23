#ifndef Creep_h_
#define Creep_h_

#include <SFML/Graphics.hpp>

namespace tg
{
    class Creep
    {
    public:
        Creep(){
            angle = 0;
            health = 100;
            totalDistance=0;
        }
        float angle;
        sf::Vector2f position;
        sf::Vector2f velocity;
        float totalDistance;
        sf::Uint32 health;
        float creationTime;
    private:
    };

};






#endif

