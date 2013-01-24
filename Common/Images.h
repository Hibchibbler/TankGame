/**********************************************************************************
Tank Game
Images.h

Copyright 2012
Daniel Ferguson 
**********************************************************************************/

#ifndef Images_h_
#define Images_h_

#include <SFML/Graphics.hpp>

namespace tg
{
    class TankImage
    {
    public:
        sf::Uint32 load(std::string bodyFn, std::string turretFn){
            //bimg = new sf::Image();
            bimg.loadFromFile(bodyFn);
            bimg.createMaskFromColor(sf::Color::Cyan,0);
            //btex = new sf::Texture();
            btex.loadFromImage(bimg);
            
            //timg = new sf::Image();
            timg.loadFromFile(turretFn);
            timg.createMaskFromColor(sf::Color::Cyan,0);
            //ttex = new sf::Texture();
            ttex.loadFromImage(timg);

            bsprite.setTexture(btex);
            tsprite.setTexture(ttex);
            return 0;
        }

        sf::Uint32 unload(){
           /* delete btex;
            delete bimg;

            delete ttex;
            delete timg;*/
            return 0;
        }

        sf::Image bimg;
        sf::Texture btex;
        sf::Sprite bsprite;

        sf::Image timg;
        sf::Texture ttex;
        sf::Sprite tsprite;
    };


    class Image
    {
    public:
        sf::Uint32 load(std::string fn){
            img.loadFromFile(fn);
            img.createMaskFromColor(sf::Color::Cyan,0);
            tex.loadFromImage(img);
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