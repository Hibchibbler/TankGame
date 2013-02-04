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

