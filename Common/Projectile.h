#ifndef Projectile_h_
#define Projectile_h_

#include <SFML/Graphics.hpp>

namespace tg
{
    class Projectile
    {
    public:
        Projectile(){
            angle = 0;
            totalDistance = 0;
            velocity= sf::Vector2f(0,0);
            position = sf::Vector2f(0,0);
            creationTime = 0.0f;
            damage = 1;
        }
        float angle;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Uint32 totalDistance;
        float creationTime;
        sf::Uint32 damage;
    private:
    };

};






#endif


