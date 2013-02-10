#ifndef Creep_h_
#define Creep_h_

#include <SFML/Graphics.hpp>
#include "ArenaManager.h"

namespace tg
{
    class Creep
    {
    public:
        Creep(){
            angle = 0;
            health = 100;
            totalDistance=0;
            lastWP = 0;
            wpType=1;
        }
        float angle;
        sf::Vector2f position;
        sf::Vector2f velocity;
        float totalDistance;
        sf::Int32 health;
        float creationTime;
        int lastWP;
        int wpType;
    private:
    };

};






#endif

