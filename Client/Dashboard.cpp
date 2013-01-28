#include "Dashboard.h"
#include "Common\LogFile.h"
#include "Common\Player.h"
#include "Common\AssetManager.h"
#include <sstream>
#include <iomanip>
using namespace tg;

sf::Uint32 Dashboard::load()
{
    LogFile::get()->log(0,0, "Dashboard::load()");
    
    dashFontColor = sf::Color::Green;
    dashPos = sf::Vector2f(0,0);
    healthTextPos = sf::Vector2f(0,0);
    speedTextPos = sf::Vector2f(0,60);
    powerTextPos = sf::Vector2f(0,100);
    posTextPos = sf::Vector2f(30,0);
    velTextPos = sf::Vector2f(30,60);
    bodyAngleTextPos = sf::Vector2f(60,0);
    turretAngleTextPos = sf::Vector2f(60,60);
    
   

    return 0;
}
sf::Uint32 Dashboard::setScore(sf::Uint32 score1, sf::Uint32 score2)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << score1 << "/" << score2;
    
    scoreText.setString(ss.str());
    scoreText.setColor(sf::Color::Green);
    scoreTextScale = sf::Vector2f(1.4f,1.4f);//sf::Vector2f(0.6,0.6);

    return 0;
}
sf::Uint32 Dashboard::setDash(Player & player)
{
    std::stringstream ss;
    
    ss << " Health: " << std::setfill(' ') << std::setw(3)  << player.tank.health;
    healthText.setString(ss.str());
    healthText.setColor(sf::Color::Green);
    healthTextScale = sf::Vector2f(0.65f,0.65f);
    ss.clear();
    ss.str("");

    ss << "  Speed: " << std::setfill(' ') << std::setw(3)  << sqrt(player.tank.velocity.x*player.tank.velocity.x+player.tank.velocity.y*player.tank.velocity.y);
    speedText.setString(ss.str());
    speedText.setColor(sf::Color::Green);
    speedTextScale = sf::Vector2f(0.65f,0.65f);
    ss.clear();
    ss.str("");

    ss << "    Power: " << std::setfill(' ') << std::setw(3)  << player.tank.power;
    powerText.setString(ss.str());
    powerText.setColor(sf::Color::Green);
    powerTextScale = sf::Vector2f(0.65f,0.65f);
    ss.clear();
    ss.str("");

    ss << " Position: " << std::setfill(' ') << std::setw(3)  << player.tank.position.x << ", " << player.tank.position.y;
    posText.setString(ss.str());
    posText.setColor(sf::Color::Green);
    posTextScale = sf::Vector2f(0.6f,0.6f);
    ss.clear();
    ss.str("");

    ss << " Velocity: " << std::setfill(' ') << std::setw(3)  << player.tank.velocity.x << ", " << player.tank.velocity.y;
    velText.setString(ss.str());
    velText.setColor(sf::Color::Green);
    velTextScale = sf::Vector2f(0.6f,0.6f);
    ss.clear();
    ss.str("");

    ss << "     Body: " << std::fixed << std::setprecision(1) << player.tank.bodyAngle << std::endl << std::endl;
    bodyAngleText.setString(ss.str());
    bodyAngleText.setColor(sf::Color::Green);
    bodyAngleTextScale = sf::Vector2f(0.6f,0.6f);
    ss.clear();
    ss.str("");

    ss << "   Turret: " << std::fixed << std::setprecision(1) << player.tank.turretAngle;
    turretAngleText.setString(ss.str());
    turretAngleText.setColor(sf::Color::Green);
    turretAngleTextScale = sf::Vector2f(0.6f,0.6f);
    ss.clear();
    ss.str("");

    return 0;
}

sf::Uint32 Dashboard::draw(sf::RenderWindow & window, AssetManager & assetMan, sf::Uint32 width, sf::Uint32 height)
{

    return 0;
}
