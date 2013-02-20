#ifndef Creep_h_
#define Creep_h_

#include <SFML/Graphics.hpp>
#include "ArenaManager.h"

namespace tg
{
    struct CreepLock{
        enum cl{
            None,
            Base,
            Turret,
            Tank,
            Creep
        };
    };


    class Creep
    {
    public:
        Creep(){
            angle = 0;
            health = 100;
            totalDistance=0;
            lastWP = 0;
            wpType=1;
            worth = 1;
            power = 1;
            damageSustained = 0;

            lock = CreepLock::None;
            locked = 0;
            detectDistance=700;
            forgetDistance=800;
            UID = 0;
            hitSuccess = 0;
            
        }

        float angle;
        sf::Vector2f position;
        sf::Vector2f velocity;
        float totalDistance;
        sf::Int32 health;
        float creationTime;
        int lastWP;
        int wpType;
        int creepType;
        int worth;
        int power;

        sf::Clock attackClock;
        sf::Clock rotateClock;

        int damageSustained;

        sf::Uint32 lock;
        sf::Uint32 locked;
        sf::Uint32 detectDistance;
        sf::Uint32 forgetDistance;

        sf::Uint32 UID;
        sf::Uint32 lockUID;
        sf::Clock lastSeenLockClock;
        sf::Clock lastLockedClock;
        sf::Uint32 hitSuccess;
    private:
    };

};






#endif

