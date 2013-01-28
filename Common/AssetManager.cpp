#include "AssetManager.h"

#include "LogFile.h"
using namespace tg;

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{

}

int AssetManager::load()
{
    font.loadFromFile("Assets\\digital-7mono.ttf");
    spriteSheet.load("Assets\\FloorTiles.png");
    smallExplosionSheet.load("Assets\\explosion2.png");
    bigExplosionSheet.load("Assets\\explosion3.png");
    //LogFile::get()->log(0,0,"Loaded explosion1");
    return 0;
}


sf::Font & AssetManager::getFont()
{
    return font;
}


sf::Sprite & AssetManager::getSprite(int i)
{
    switch (i)
    {
    case ImageType::Projectile1:
        spriteSheet.sprite.setTextureRect(sf::IntRect(448,64,32,32));
        return spriteSheet.sprite;
        break;
    case ImageType::ProjectileShadow:
        return spriteSheet.sprite;
        break;
    case ImageType::ProjectileHealRay:
        spriteSheet.sprite.setTextureRect(sf::IntRect(448,0,64,64));
        return spriteSheet.sprite;
        break;
    case ImageType::ProjectileDeathRay:
        spriteSheet.sprite.setTextureRect(sf::IntRect(384,0,64,64));
        return spriteSheet.sprite;
        break;
    case ImageType::FloorWall1:
        spriteSheet.sprite.setTextureRect(sf::IntRect(0,0,128,128));
        return spriteSheet.sprite;
        break;
    case ImageType::Floor1:
        spriteSheet.sprite.setTextureRect(sf::IntRect(128,0,128,128));
        return spriteSheet.sprite;
        break;
    case ImageType::Floor2:
        spriteSheet.sprite.setTextureRect(sf::IntRect(256,0,128,128));
        return spriteSheet.sprite;
        break;
    case ImageType::FloorBase1:
        spriteSheet.sprite.setTextureRect(sf::IntRect(0,128,128,128));
        return spriteSheet.sprite;
        break;
    case ImageType::FloorBase2:
        spriteSheet.sprite.setTextureRect(sf::IntRect(128,128,128,128));
        return spriteSheet.sprite;
        break;
    case ImageType::FloorGenerator1:
        spriteSheet.sprite.setTextureRect(sf::IntRect(0,256,128,128));
        return spriteSheet.sprite;
        break;
    case ImageType::FloorGenerator2:
        spriteSheet.sprite.setTextureRect(sf::IntRect(128,256,128,128));
        return spriteSheet.sprite;
        break;
    case ImageType::Minion1:
        spriteSheet.sprite.setTextureRect(sf::IntRect(256,323,57,70));
        return spriteSheet.sprite;
        break;
    case ImageType::Minion2:
        spriteSheet.sprite.setTextureRect(sf::IntRect(319,327,66,62));
        return spriteSheet.sprite;
        break;
    case ImageType::Dash1:
        spriteSheet.sprite.setTextureRect(sf::IntRect(0,447,512,65));
        return spriteSheet.sprite;
        break;
    case ImageType::Explosion1:
        return smallExplosionSheet.sprite;
        break;
    case ImageType::Explosion2:
        return bigExplosionSheet.sprite;
        break;
    case ImageType::TankBlue:
        spriteSheet.sprite.setTextureRect(sf::IntRect(256,128,73,116));
        return spriteSheet.sprite;
        break;
    case ImageType::TankRed:
        spriteSheet.sprite.setTextureRect(sf::IntRect(384,128,73,116));
        return spriteSheet.sprite;
        break;
    case ImageType::TankShadow:
        return spriteSheet.sprite;
        break;
    case ImageType::TurretBlue:
        spriteSheet.sprite.setTextureRect(sf::IntRect(331,128,47,164));
        return spriteSheet.sprite;
        break;
    case ImageType::TurretRed:
        spriteSheet.sprite.setTextureRect(sf::IntRect(459,128,47,164));
        return spriteSheet.sprite;
        break;
    case ImageType::TurretShadow:
        return spriteSheet.sprite;
        break;
    }
}

