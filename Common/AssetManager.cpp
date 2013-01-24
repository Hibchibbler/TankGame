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
    tank_images[TankImageType::TankBlue].load("Assets\\BlueTankBody.png", "Assets\\BlueTankTurret.png");// = ti;
    LogFile::get()->log(0,0,"Loaded blue tank");
    
    tank_images[TankImageType::TankRed].load("Assets\\RedTankBody.png", "Assets\\RedTankTurret.png");// = ti;
    LogFile::get()->log(0,0,"Loaded red tank");
    
    tank_images[TankImageType::TankShadow].load("Assets\\ShadowTankBody.png", "Assets\\ShadowTankTurret.png");
    LogFile::get()->log(0,0,"Loaded Shadow tank");

    //Load projectiles
    single_images[ImageType::Projectile1].load("Assets\\projectile.png");
    LogFile::get()->log(0,0,"Loaded projectile");

    single_images[ImageType::ProjectileShadow].load("Assets\\projectileShadow.png");
    LogFile::get()->log(0,0,"Loaded projectile shadow");

    single_images[ImageType::ProjectileDeathRay].load("Assets\\baselaser1.png");
    LogFile::get()->log(0,0,"Loaded Base Laser");

    single_images[ImageType::ProjectileHealRay].load("Assets\\heallaser.png");
    LogFile::get()->log(0,0,"Loaded Heal Laser");

    //Load floor tiles
    single_images[ImageType::FloorWall1].load("Assets\\floor1.png");
    LogFile::get()->log(0,0,"Loaded floor1");//0

    single_images[ImageType::Floor1].load("Assets\\floor2.png");
    LogFile::get()->log(0,0,"Loaded floor2");//1

    single_images[ImageType::Floor2].load("Assets\\floor3.png");
    LogFile::get()->log(0,0,"Loaded floor3");//2

    single_images[ImageType::FloorBase1].load("Assets\\team1Garage.png");
    LogFile::get()->log(0,0,"Loaded team1Garage");//3

    single_images[ImageType::FloorBase2].load("Assets\\team2Garage.png");
    LogFile::get()->log(0,0,"Loaded team2Garage");//4

    single_images[ImageType::FloorGenerator1].load("Assets\\team1Generator.png");
    LogFile::get()->log(0,0,"Loaded team1Generator");//5

    single_images[ImageType::FloorGenerator2].load("Assets\\team2Generator.png");
    LogFile::get()->log(0,0,"Loaded team2Generator");//6

    single_images[ImageType::FloorTiles].load("Assets\\FloorTiles.png");

    //load minions
    single_images[ImageType::Minion1].load("Assets\\minion1.png");
    LogFile::get()->log(0,0,"Loaded minion1");

    single_images[ImageType::Minion2].load("Assets\\minion2.png");
    LogFile::get()->log(0,0,"Loaded minion2");

    single_images[ImageType::Dash1].load("Assets\\dashboard1.png");
    LogFile::get()->log(0,0,"Loaded dashboard1");

    single_images[ImageType::Explosion1].load("Assets\\explosion2.png");
    LogFile::get()->log(0,0,"Loaded explosion1");

    single_images[ImageType::Explosion2].load("Assets\\explosion3.png");
    LogFile::get()->log(0,0,"Loaded explosion2");

    return 0;
}

TankImage & AssetManager::getTankImage(int i)
{
    return tank_images[i];
}
Image & AssetManager::getImage(int i)
{
    return single_images[i];
}


sf::Font & AssetManager::getFont()
{
    return font;
}


