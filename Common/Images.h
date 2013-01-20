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
            bimg = new sf::Image();
            bimg->loadFromFile(bodyFn);
            bimg->createMaskFromColor(sf::Color::Cyan,0);
            btex = new sf::Texture();
            btex->loadFromImage(*bimg);
            
            timg = new sf::Image();
            timg->loadFromFile(turretFn);
            timg->createMaskFromColor(sf::Color::Cyan,0);
            ttex = new sf::Texture();
            ttex->loadFromImage(*timg);
            return 0;
        }

        sf::Uint32 unload(){
            delete btex;
            delete bimg;

            delete ttex;
            delete timg;
            return 0;
        }

        sf::Image* bimg;
        sf::Texture* btex;

        sf::Image* timg;
        sf::Texture* ttex;
    };

    class FloorImage
    {
    public:
        int load(std::string fn){
            img = new sf::Image();
            img->loadFromFile(fn);
            img->createMaskFromColor(sf::Color::Cyan,0);
            tex = new sf::Texture();
            tex->loadFromImage(*img);
            return 0;
        }
        sf::Image* img;
        sf::Texture* tex;
    };

    class ProjectileImage
    {
    public:
        sf::Uint32 load(std::string fn){
            img = new sf::Image();
            img->loadFromFile(fn);
            img->createMaskFromColor(sf::Color::Cyan,0);
            
            tex = new sf::Texture();
            tex->loadFromImage(*img);
            
            return 0;
        }
        sf::Uint32 unload(){
            delete tex;
            delete img;
        }
        sf::Image* img;
        sf::Texture* tex;
    };

    class Image
    {
    public:
        sf::Uint32 load(std::string fn){
            img = new sf::Image();
            img->loadFromFile(fn);
            img->createMaskFromColor(sf::Color::Cyan,0);
            tex = new sf::Texture();
            tex->loadFromImage(*img);
            return 0;
        }
        sf::Uint32 unload(){
            delete tex;
            delete img;
        }
        sf::Image* img;
        sf::Texture* tex;
    };

  

    class MinionImage
    {
    public:
        sf::Uint32 load(std::string fn){
            img = new sf::Image();
            img->loadFromFile(fn);
            img->createMaskFromColor(sf::Color::Cyan,0);
            tex = new sf::Texture();
            tex->loadFromImage(*img);
            return 0;
        }
        sf::Image* img;
        sf::Texture* tex;
    };


    class MinionSprite
    {
    public:
        sf::Sprite sprite;
    };

    class TankSprite
    {
    public:
        sf::Sprite sprite;
    };

    class ProjectileSprite
    {
    public:
        sf::Sprite sprite;
    };

    class FloorSprite
    {
    public:
        FloorSprite(){
            id = 0;
        }
        sf::Uint32 id;
        sf::Sprite sprite;
    };

    class ImageAsset
    {
    public:
        sf::Uint32 load(std::string fn){
            i.loadFromFile(fn);
            i.createMaskFromColor(sf::Color::Cyan,0);
            t.loadFromImage(i);
            return 0;
        }

        sf::Texture & getTexture(){
            return t;
        }
        sf::Image & getImage(){return i;}
    private:
        sf::Image i;
        sf::Texture t;

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
        }
        sf::Uint32 type;
        sf::Vector2f position;
        sf::Uint32 index;
        sf::Clock rate;
    };
};

#endif