#ifndef StageLobby_h_
#define StageLobby_h_

#include "Common\GameStage.h"

namespace tg
{
    class StageLobby : public GameStage
    {
    public:
        StageLobby();
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);

        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
    private:
    };
};



#endif


/*


class LobbyStage : public IGameStage
{
public:
    sf::Uint32 onRemoteEvent(sg::Comm *comm, sg::CommEvent *e){
        return 0;
    }
    sf::Uint32 onWindowEvent(sf::RenderWindow * w, sf::Event &e){
        
        //Pass it down to textbox which wants to know if it was clicked in or not.
        if (e.type == sf::Event::MouseButtonPressed){
            name1.processMouseButtonPressed(*w,e);
        }else if (e.type == sf::Event::KeyPressed){
            name1.processKeypress(e);
        }
        
        return 0;
    }
    sf::Uint32 doInit(sg::Comm *comm, int w, int h){
        lobbyBackground.load("lobby_background.png");
        name1.load();
        
        name1.scale(sf::Vector2f(1.09,1.0));
        name1.setString(std::string("Danimal"));
        //lobbyBackground.getImage().createMaskFromColor(sf::Color::Cyan,0);
        lobbyBackgroundSprite.setTexture(lobbyBackground.getTexture());
        return 0;
    }
    sf::Uint32 doLocalInput(){
        return 0;
    }
    sf::Uint32 doLoop(){
        return 0;
    }
    sf::Uint32 doCleanup(){
        return 0;
    }

    void draw(sf::RenderTarget & target, sf::RenderStates states) const {
        target.draw(lobbyBackgroundSprite,states);
        target.draw(name1,states);
    }


private:
    TextBox name1;
    ImageAsset lobbyBackground;
    sf::Sprite lobbyBackgroundSprite;
private:
};
*/














