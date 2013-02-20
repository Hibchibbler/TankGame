/**********************************************************************************
Tank Game
Images.h

Copyright 2012
Daniel Ferguson 
**********************************************************************************/

#ifndef Images_h_
#define Images_h_

#include <SFML/Graphics.hpp>
#include <iostream>

namespace tg
{
    struct ImageType{
        enum siat{
            Projectile1,
            ProjectileShadow,
            ProjectileHealRay,
            ProjectileDeathRay,
            FloorWall1,
            Floor1,
            Floor2,
            Base1,
            Base2,
            Generator1,
            Generator2,
            Waypoint1_1,
            Waypoint2_1,
            Waypoint3_1,
            Waypoint1_2,
            Waypoint2_2,
            Waypoint3_2,
            FogOfWar15,
            FogOfWar30,
            FogOfWar45,
            FogOfWar60,
            FogOfWar75,
            Ghost,
            Minion1,
            Minion2,
            Minion3,
            Minion4,
            Minion5,
            Minion6,
            Minion7,
            Minion8,
            Dash1,
            ExplosionSmall,
            ExplosionBig,
            TankBlue,
            TankRed,
            TankShadow,
            TurretBlue,
            TurretRed,
            TurretShadow,
            MAX_IT
        };
    };


    class Image
    {
    public:
        sf::Uint32 load(std::string fn){
            std::cout << "sf::Texture::getMaximumSize() == " << sf::Texture::getMaximumSize() << std::endl;

            img.loadFromFile(fn);
            
            img.createMaskFromColor(sf::Color::Cyan,0);
            
            tex.loadFromImage(img);
            tex.setSmooth(false);
            std::cout << tex.getSize().x << ", " << tex.getSize().y <<  std::endl;
            sprite.setTexture(tex);
            
            return 0;
        }
        sf::Uint32 unload(){

        }
        sf::Image img;
        sf::Texture tex;
        sf::Sprite sprite;
    };

    struct ExplosionType{
        enum et{
            None,
            TankHit,
            TankDeath,
            CreepHit,
            CreepDeath,
            GeneratorHit,
            GeneratorDeath,
            BaseHit,
            BaseDeath
        };
    };

    class Explosion
    {
    public:
        Explosion(){
            index = 0;
            type = ExplosionType::None;
            repeat=false;
        }
        sf::Uint32 type;
        sf::Vector2f position;
        sf::Uint32 index;
        sf::Clock rate;
        bool repeat;
    };
};

#endif

