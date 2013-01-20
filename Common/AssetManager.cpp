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
    TankImage ti;
    ti.load("Assets\\BlueTankBody.png", "Assets\\BlueTankTurret.png");
    tankImages["BlueTank"] = ti;//.push_back(ti);
    LogFile::get()->log(0,0,"Loaded blue tank");

    ti.load("Assets\\RedTankBody.png", "Assets\\RedTankTurret.png");
    tankImages["RedTank"] = ti;//.push_back(ti);
    LogFile::get()->log(0,0,"Loaded red tank");

     ti.load("Assets\\GreenTankBody.png", "Assets\\GreenTankTurret.png");
    tankImages["GreenTank"] = ti;//.push_back(ti);
    LogFile::get()->log(0,0,"Loaded green tank");

    ti.load("Assets\\YellowTankBody.png", "Assets\\YellowTankTurret.png");
    tankImages["YellowTank"] = ti;//.push_back(ti);
    LogFile::get()->log(0,0,"Loaded yellow tank");

    ti.load("Assets\\ShadowTankBody.png", "Assets\\ShadowTankTurret.png");
    tankImages["ShadowTank"] = ti;//.push_back(ti);
    LogFile::get()->log(0,0,"Loaded Shadow tank");

    //Load projectiles
    ProjectileImage pi;
    pi.load("Assets\\projectile.png");
    projectileImages["Projectile"] = pi;//.push_back(pi);
    LogFile::get()->log(0,0,"Loaded projectile");

    pi.load("Assets\\projectileShadow.png");
    projectileImages["ProjectileShadow"] = pi;//.push_back(pi);
    LogFile::get()->log(0,0,"Loaded projectile shadow");

    ProjectileImage pi2;
    pi2.load("Assets\\baselaser1.png");
    projectileImages["BaseLaser"] = pi2;//.push_back(pi);
    LogFile::get()->log(0,0,"Loaded Base Laser");

    ProjectileImage pi3;
    pi3.load("Assets\\heallaser.png");
    projectileImages["HealLaser"] = pi3;//.push_back(pi);
    LogFile::get()->log(0,0,"Loaded Heal Laser");

    //Load floor tiles
    FloorImage fi;
    fi.load("Assets\\floor1.png");
    floorImages["Floor1"] = fi;//.push_back(fi);
    LogFile::get()->log(0,0,"Loaded floor1");

    fi.load("Assets\\floor2.png");
    floorImages["Floor2"] = fi;//.push_back(fi);
    LogFile::get()->log(0,0,"Loaded floor2");

    fi.load("Assets\\floor3.png");
    floorImages["Floor3"] = fi;//.push_back(fi);
    LogFile::get()->log(0,0,"Loaded floor3");

    fi.load("Assets\\team1Garage.png");
    floorImages["Team1Garage"] = fi;//.push_back(fi);
    LogFile::get()->log(0,0,"Loaded team1Garage");

    fi.load("Assets\\team2Garage.png");
    floorImages["Team2Garage"] = fi;//.push_back(fi);
    LogFile::get()->log(0,0,"Loaded team2Garage");

    fi.load("Assets\\team1Generator.png");
    floorImages["Team1Generator"] = fi;//.push_back(fi);
    LogFile::get()->log(0,0,"Loaded team1Generator");

    fi.load("Assets\\team2Generator.png");
    floorImages["Team2Generator"] = fi;//.push_back(fi);
    LogFile::get()->log(0,0,"Loaded team2Generator");

    //load minions
    MinionImage mi;
    mi.load("Assets\\minion1.png");
    minionImages["Minion1"] = mi;//.push_back(mi);
    LogFile::get()->log(0,0,"Loaded minion1");

    mi.load("Assets\\minion2.png");
    minionImages["Minion2"] = mi;//.push_back(mi);
    LogFile::get()->log(0,0,"Loaded minion2");


    
    dashboardImage.load("Assets\\dashboard1.png");
    LogFile::get()->log(0,0,"Loaded dashboard1");


    //tankExplosionImage.load("Assets\\tank_explosion2.png");
    tankDeathExplosionImage.load("Assets\\explosion2.png");
    tankHitExplosionImage.load("Assets\\explosion3.png");

    return 0;
}

Image & AssetManager::getTankDeathExplosionImage()
{
    return tankDeathExplosionImage;
}

Image & AssetManager::getTankHitExplosionImage()
{
    return tankHitExplosionImage;
}


TankImage & AssetManager::getTankImage(std::string name)
{
    return tankImages[name];
}
FloorImage & AssetManager::getFloorImage(std::string name)
{
    return floorImages[name];
}
ProjectileImage & AssetManager::getProjectileImage(std::string name)
{
    return projectileImages[name];
}
MinionImage & AssetManager::getMinionImage(std::string name)
{
    return minionImages[name];
}

sf::Font & AssetManager::getFont()
{
    return font;
}

Image & AssetManager::getDashboardImage()
{
    return dashboardImage;
}

