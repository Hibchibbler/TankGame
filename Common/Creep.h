#ifndef Creep_h_
#define Creep_h_

#include <SFML/Graphics.hpp>

namespace tg
{
    class Creep
    {
    public:
        float angle;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Uint32 totalDistance;
    private:
    };

};






#endif

