#include "AssetManager.h"

#include "LogFile.h"

int tg::AssetManager::load(){
    //Load Tanks
    TankImage ti;
    ti.load("Assets\\BlueBody_red.png", "Assets\\BlueTurret.png");
    tankImages["BlueTank"] = ti;//.push_back(ti);
    LogFile::get()->log(0,0,"Loaded blue tank");

    ti.load("Assets\\RedBody_red.png", "Assets\\RedTurret.png");
    tankImages["RedTank"] = ti;//.push_back(ti);
    LogFile::get()->log(0,0,"Loaded red tank");

    //Load projectiles
    ProjectileImage pi;
    pi.load("Assets\\projectile.png");
    projectileImages["Projectile"] = pi;//.push_back(pi);
    LogFile::get()->log(0,0,"Loaded projectile");

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
    return 0;
}

