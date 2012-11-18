#ifndef GameStage_h_
#define GameStage_h_


#include <SFML/Graphics.hpp>

#define SUMMARY_ELEMENTS 16
#define INPUT_ELEMENTS 16
namespace tg
{
    class Comm;
    class CommEvent;
    class TeamManager;
    class ArenaManager;
    class GameServer;
    //This may have to be server and client specific....
    class GameStage
    {
    public:
        GameStage();
        virtual ~GameStage();
        virtual sf::Uint32 doRemoteEvent(TeamManager & teamMan, 
                                         ArenaManager & arenaMan,
                                         CommEvent & cevent,
                                         sf::Uint32 connId,
                                         sf::Uint32 msgId);

        virtual sf::Uint32 doWindowEvent(sf::RenderWindow & w, 
                                          sf::Event & event);

        virtual sf::Uint32 doInit(int w, int h);
        virtual sf::Uint32 doLocalInput(sf::RenderWindow &window, TeamManager & teamMan);
        virtual sf::Uint32 doLoop(Comm & comm, TeamManager & teamMan);
        virtual sf::Uint32 doCleanup();
        virtual sf::Uint32 doDraw(sf::RenderWindow &window);

        void setId(sf::Uint32 id);
        sf::Uint32 getId();
        
        sf::Uint32 getSummary(sf::Uint16 i);
        void setInput(sf::Uint32 s, sf::Uint16 i);
        
    protected:
        void setSummary(sf::Uint32 s, sf::Uint16 i);
        sf::Uint32 getInput(sf::Uint16 i);
    private:    
        sf::Uint32 id;
        sf::Uint32 summary[SUMMARY_ELEMENTS];
        sf::Uint32 inputs[INPUT_ELEMENTS];
    };

};


#endif

