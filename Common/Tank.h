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
            health = 100;
            power = 1;
            throttle = 0.0f;
            bodyAngle = 0.0f;
            turretAngle = 0.0f;
            mass = 5.0f;
            shadowUpdated = false;
            prevDist = 0.0f;
            curDist = 0.0f;
        }
        sf::Int32 health;
        sf::Uint32 power;

        sf::Vector2f position;
        sf::Vector2f velocity;
        float throttle;
        float bodyAngle;
        float turretAngle;
        float mass;

        sf::Vector2f shadowPos;
        sf::Vector2f shadowVel;
        bool shadowUpdated;
        float dx;
        float dy;
        float accx;
        float accy;
        float prevDist;
        float curDist;
        
    };

};

#endif

