#ifndef Tank_h_
#define Tank_h_

#include <SFML/System.hpp>
#include <list>

namespace tg
{
    struct AttackAction{
        enum aa{
            Attacking,
            Idle
        };
    };

    struct AccelerateAction{
        enum aa{
            Forward,
            Reverse,
            Idle
        };
    };

    struct TurnAction{
        enum ta{
            Right,
            Left,
            Idle
        };
    };

    class Tank
    {
    public:
        Tank(){
            position = sf::Vector2f(200,200);
            velocity = sf::Vector2f(0,0);
            health = maxHealth = 100;
            power = 1;
            throttle = 0.0f;
            bodyAngle = 0.0f;
            turretAngle = 0.0f;
            mass = 5.0f;
            shadowUpdated = false;
            prevDist = 0.0f;
            curDist = 0.0f;
            attacking = AttackAction::Idle;
            accelerating= AccelerateAction::Idle;
            turningBody = TurnAction::Idle;
            explosionIndex = 0;
        }
        sf::Int32 maxHealth;
        sf::Int32 health;
        sf::Uint32 power;



        sf::Vector2f position;
        sf::Vector2f velocity;
        float throttle;
        float bodyAngle;
        float turretAngle;
        float mass;
        sf::Uint32 attacking;
        sf::Uint32 accelerating;
        sf::Uint32 turningBody;

        sf::Vector2f shadowPos;
        sf::Vector2f shadowVel;
        bool shadowUpdated;
        float dx;
        float dy;
        float accx;
        float accy;
        float prevDist;
        float curDist;
        
        
        sf::Clock explosionClock;
        sf::Uint32 explosionIndex;
    };

};

#endif

