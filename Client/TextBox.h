#ifndef TextBox_h_
#define TextBox_h_

#include <SFML/Graphics.hpp>

namespace tg
{
    class Image;

    class TextBox : public sf::Drawable
    {
    public:
        TextBox();
        sf::Uint32 load();
        sf::Uint32 processMouseButtonPressed(sf::RenderWindow & w, sf::Event &e);

        sf::Uint32 processKeypress(sf::Event &e);
        void scale(sf::Vector2f &m);

        void setPosition(sf::Vector2f position);

        void setString(std::string &s);

   
    private:
        bool hasFocus;
        sf::Font font;
        sf::Text aString;
        //Image backgroundAsset;
        sf::Sprite backgroundSp;
        void draw(sf::RenderTarget & target, sf::RenderStates states) const;
    
        std::string getASCII(sf::Keyboard::Key k);
    };

};
#endif








