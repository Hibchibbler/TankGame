#include "AssetManager.h"

#include "LogFile.h"
using namespace tg;

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
    /*tankImages
    projectileImages
    floorImages
    minionImages*/
    //TODO: Must unload these!!
}

int AssetManager::load(){
    /*Current names get the gets.
    BlueTank
    RedTank
    -------
    Projectile
    -------
    Floor1
    Floor3
    Team1Garage
    Team2Garage
    Team1Generator
    Team2Generator
    -------
    Minion1
    Minion2
    */
    font.loadFromFile("Assets\\digital-7mono.ttf");
    
    //Load Tanks
    //tank_images[TankImageType::TankBlue].load("Assets\\BlueTankBody.png", "Assets\\BlueTankTurret.png");// = ti;
    //LogFile::get()->log(0,0,"Loaded blue tank");
    //
    //tank_images[TankImageType::TankRed].load("Assets\\RedTankBody.png", "Assets\\RedTankTurret.png");// = ti;
    //LogFile::get()->log(0,0,"Loaded red tank");
    //
    //tank_images[TankImageType::TankShadow].load("Assets\\ShadowTankBody.png", "Assets\\ShadowTankTurret.png");
    //LogFile::get()->log(0,0,"Loaded Shadow tank");


    //Load projectiles
    //single_images[ImageType::Projectile1].load("Assets\\projectile.png");
    //LogFile::get()->log(0,0,"Loaded projectile");

    //single_images[ImageType::ProjectileShadow].load("Assets\\projectileShadow.png");
    //LogFile::get()->log(0,0,"Loaded projectile shadow");

    //single_images[ImageType::ProjectileDeathRay].load("Assets\\baselaser1.png");
    //LogFile::get()->log(0,0,"Loaded Base Laser");

    //single_images[ImageType::ProjectileHealRay].load("Assets\\heallaser.png");
    //LogFile::get()->log(0,0,"Loaded Heal Laser");

    ////Load floor tiles
    //single_images[ImageType::FloorWall1].load("Assets\\floor1.png");
    //LogFile::get()->log(0,0,"Loaded floor1");//0

    //single_images[ImageType::Floor1].load("Assets\\floor2.png");
    //LogFile::get()->log(0,0,"Loaded floor2");//1

    //single_images[ImageType::Floor2].load("Assets\\floor3.png");
    //LogFile::get()->log(0,0,"Loaded floor3");//2

    //single_images[ImageType::FloorBase1].load("Assets\\team1Garage.png");
    //LogFile::get()->log(0,0,"Loaded team1Garage");//3

    //single_images[ImageType::FloorBase2].load("Assets\\team2Garage.png");
    //LogFile::get()->log(0,0,"Loaded team2Garage");//4

    //single_images[ImageType::FloorGenerator1].load("Assets\\team1Generator.png");
    //LogFile::get()->log(0,0,"Loaded team1Generator");//5

    //single_images[ImageType::FloorGenerator2].load("Assets\\team2Generator.png");
    //LogFile::get()->log(0,0,"Loaded team2Generator");//6

    //single_images[ImageType::FloorTiles].load("Assets\\FloorTiles.png");
    spriteSheet.load("Assets\\FloorTiles.png");

    ////load minions
    //single_images[ImageType::Minion1].load("Assets\\minion1.png");
    //LogFile::get()->log(0,0,"Loaded minion1");

    //single_images[ImageType::Minion2].load("Assets\\minion2.png");
    //LogFile::get()->log(0,0,"Loaded minion2");

    //single_images[ImageType::Dash1].load("Assets\\dashboard1.png");
    //LogFile::get()->log(0,0,"Loaded dashboard1");

    //single_images[ImageType::Explosion1]
    smallExplosionSheet.load("Assets\\explosion2.png");
    bigExplosionSheet.load("Assets\\explosion3.png");
    //LogFile::get()->log(0,0,"Loaded explosion1");

    //single_images[ImageType::Explosion2].load("Assets\\explosion3.png");
    //LogFile::get()->log(0,0,"Loaded explosion2");

    ////Load Tanks
    //single_images[ImageType::TankBlue].load("Assets\\BlueTankBody.png");
    //LogFile::get()->log(0,0,"Loaded BlueTankBody");

    //single_images[ImageType::TurretBlue].load("Assets\\BlueTankTurret.png");
    //LogFile::get()->log(0,0,"Loaded BlueTankTurret");

    //single_images[ImageType::TankRed].load("Assets\\RedTankBody.png");
    //LogFile::get()->log(0,0,"Loaded RedTankBody");

    //single_images[ImageType::TurretRed].load("Assets\\RedTankTurret.png");
    //LogFile::get()->log(0,0,"Loaded RedTankTurret");

    //single_images[ImageType::TankShadow].load("Assets\\ShadowTankBody.png");
    //LogFile::get()->log(0,0,"Loaded ShadowTankBody");

    //single_images[ImageType::TurretShadow].load("Assets\\ShadowTankTurret.png");
    //LogFile::get()->log(0,0,"Loaded ShadowTankTurret");
    return 0;
}

//TankImage & AssetManager::getTankImage(int i)
//{
//    return tank_images[i];
//}
//Image & AssetManager::getImage(int i)
//{
//    return single_images[i];
//}


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
        spriteSheet.sprite.setTextureRect(sf::IntRect(384,128,73,128));
        return spriteSheet.sprite;
        break;
    case ImageType::TankShadow:
        return spriteSheet.sprite;
        break;
    case ImageType::TurretBlue:
        spriteSheet.sprite.setTextureRect(sf::IntRect(329,128,53,180));
        return spriteSheet.sprite;
        break;
    case ImageType::TurretRed:
        spriteSheet.sprite.setTextureRect(sf::IntRect(458,128,53,180));
        return spriteSheet.sprite;
        break;
    case ImageType::TurretShadow:
        return spriteSheet.sprite;
        break;
    }

    
}

