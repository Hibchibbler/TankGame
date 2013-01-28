#ifndef GameStage_h_
#define GameStage_h_


#include <SFML/Graphics.hpp>
#include <string>

#define SUMMARY_ELEMENTS 16
#define INPUT_ELEMENTS 16
namespace tg
{
    class CommEvent;
    class Game;
    
    class Element{
    public:
        sf::Uint32 a;
        std::string b;
    };

    class GameStage
    {
    public:
        GameStage(Game & game);
        virtual ~GameStage();
        virtual sf::Uint32 doRemoteEvent(Game & g,
                                         CommEvent & cevent,
                                         sf::Uint32 connId,
                                         sf::Uint32 msgId);

        virtual sf::Uint32 doWindowEvent(sf::RenderWindow & w, 
                                          sf::Event & event);

        virtual sf::Uint32 doInit(Game & g);
        virtual sf::Uint32 doLocalInput(sf::RenderWindow &window, Game & g);
        virtual sf::Uint32 doLoop(Game & g);
        virtual sf::Uint32 doCleanup(Game & g);
        virtual sf::Uint32 doDraw(sf::RenderWindow &window,Game & g, sf::Time ft);//TODO: carry this inside. 

        void setId(sf::Uint32 id);
        sf::Uint32 getId();

        Element getSummary(sf::Uint16 i);
        void setInput(Element e, sf::Uint16 i);
        Game & getOwner();
    protected:
        void setSummary(Element e, sf::Uint16 i);
        Element getInput(sf::Uint16 i);
        Game & g;
    private:    
        sf::Uint32 id;
        Element summary[SUMMARY_ELEMENTS];
        Element inputs[INPUT_ELEMENTS];
    };

};


#endif

