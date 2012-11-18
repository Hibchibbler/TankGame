#ifndef Tank_h_
#define Tank_h_

#include <SFML/System.hpp>

namespace tg
{
    class Tank
    {
    public:
        Tank(){
            position = sf::Vector2f(200,200);
            velocity = sf::Vector2f(0,0);
            throttle = 0.0f;
            bodyAngle = 0.0f;
            turretAngle = 0.0f;
            mass = 5.0f;
        }

        sf::Vector2f position;
        sf::Vector2f velocity;
        float throttle;
        float bodyAngle;
        float turretAngle;
        float mass;
    };

};

#endif

