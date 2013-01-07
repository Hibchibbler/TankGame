#include "TextBox.h"

using namespace tg;

TextBox::TextBox(){

}
sf::Uint32 TextBox::load(){
    hasFocus = false;
    font.loadFromFile("digital-7mono.ttf");
    aString.setFont(font);
    aString.setColor(sf::Color::Green);
    aString.scale(0.96f,0.80f);
    //backgroundAsset.load("textbox_background2.png");
    //backgroundSp.setTexture(backgroundAsset.getTexture());

    //backgroundSp.setPosition(sf::Vector2f(57,59));
    aString.setPosition(sf::Vector2f(63.0f,58.0f));
    return 0;
}
sf::Uint32 TextBox::processMouseButtonPressed(sf::RenderWindow & w, sf::Event &e)
{
    sf::Vector2f wc;
    wc = w.convertCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y));

    //Hit test text box. using background png as boundary.
    if (backgroundSp.getGlobalBounds().contains(wc)){
        std::string s = aString.getString();
        if (hasFocus == false)
            s.append("_");
        hasFocus = true;
        aString.setString(s);
    }else{
        std::string s = aString.getString();
        if (hasFocus == true)
            s.pop_back();
        aString.setString(s);
        hasFocus = false;
    }
    return 0;
}

sf::Uint32 TextBox::processKeypress(sf::Event &e)
{
    if (e.key.code >= sf::Keyboard::A &&
        e.key.code <= sf::Keyboard::Z){
        if (hasFocus){
            std::string s = aString.getString();
            if (s.size() < 13){
                s.pop_back();//.erase(s.end()-1);
                s.append(getASCII(e.key.code));
                s.append("_");
                aString.setString(s);
            }
        }
    }else if (e.key.code == sf::Keyboard::BackSpace){
        if (hasFocus){
            std::string s = aString.getString();
            if (s.size() > 1){
                s.pop_back();
                s.pop_back();
                s.append("_");
                aString.setString(s);
            }
        }
    }
    return 0;
}
void TextBox::scale(sf::Vector2f &m){
    backgroundSp.scale(m);
}

void TextBox::setPosition(sf::Vector2f position){
    backgroundSp.setPosition(position);
    aString.setPosition(position);
}

void TextBox::setString(std::string &s){
    aString.setString(s);
}






void TextBox::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    //First draw the textbox background
    target.draw(backgroundSp, states);
    //then the string
    target.draw(aString, states);
}
    
std::string TextBox::getASCII(sf::Keyboard::Key k){
    sf::Uint32 mk = k;
    mk += 62;
    std::string s;
    s.push_back(mk);
    return s;
}














