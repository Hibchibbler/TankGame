#ifndef Dashboard_h_
#define Dashboard_h_

#include <SFML/Graphics.hpp>
namespace tg
{
    class Player;
    class AssetManager;

    class Dashboard
    {
    public:
        sf::Uint32 load();
        sf::Uint32 setDash(Player & player);
        sf::Uint32 draw(sf::RenderWindow & window, AssetManager & assetMan, sf::Uint32 width, sf::Uint32 height);

        //sf::View dashView;
        sf::Sprite backDrop;
        
        sf::Vector2f dashPos;
        sf::Vector2f dashScale;
        sf::Color    dashFontColor;
        
        sf::Text     healthText;
        sf::Vector2f healthTextScale;
        sf::Vector2f healthTextPos;
        
        sf::Text     speedText;
        sf::Vector2f speedTextScale;
        sf::Vector2f speedTextPos;

        sf::Text     powerText;
        sf::Vector2f powerTextScale;
        sf::Vector2f powerTextPos;
        
        sf::Text     posText;
        sf::Vector2f posTextScale;
        sf::Vector2f posTextPos;

        sf::Text     velText;
        sf::Vector2f velTextScale;
        sf::Vector2f velTextPos;
        
        sf::Text     bodyAngleText;
        sf::Vector2f bodyAngleTextScale;
        sf::Vector2f bodyAngleTextPos;

        sf::Text     turretAngleText;
        sf::Vector2f turretAngleTextScale;
        sf::Vector2f turretAngleTextPos;

    };

};
#endif



