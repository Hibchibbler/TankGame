#ifndef StageRun_h_
#define StageRun_h_

#include "Common\GameStage.h"
#include "Common\Player.h"
#include "Common\Images.h"
#include "Dashboard.h"
#include <SFGUI/SFGUI.hpp>

namespace tg
{
    struct TextureSubInfo{
        TextureSubInfo(sf::Vector2f p, sf::IntRect r, float a){
            position = p;
            rect     = r;
            angle    = a;
        }
        sf::Vector2f position;
        sf::IntRect rect;
        float angle;
    };

    class StageRun : public GameStage
    {
    public:
        StageRun(Game & g);
        sf::Uint32 doInit(Game & g);
        sf::Uint32 doRemoteEvent(Game & g,
                                 CommEvent & cevent,
                                 sf::Uint32 connId,
                                 sf::Uint32 msgId);
        sf::Uint32 doWindowEvent(sf::RenderWindow & w, 
                                 sf::Event & event);
        sf::Uint32 doLoop(Game & g);
        sf::Uint32 doLocalInput(sf::RenderWindow & window, Game & g);
        sf::Uint32 doDraw(sf::RenderWindow & window, Game & g, sf::Time ft);
    private:
        void doQuit();
        bool showRunMenu;
        sfg::Window::Ptr mywindow;

        sf::Uint32 prepareAssets(Game &g);
        sf::Uint32 drawAll(sf::RenderWindow & window, Game & g);
        Dashboard dash;
        Player thisPlayer;
        sf::Vector2f targetPosition;

        //float arenaViewZoom;
        sf::View arenaView;
        sf::View dashView;
        float zoom;
        float zoomPow;
        sf::Uint32 visionRange;
        
        sf::Uint32 scrWidth;
        sf::Uint32 scrHeight;

        sf::Uint32 attacking;
        sf::Vector2i curMousePos;
        sf::Vector2i lastMousePos;
        float curTurretAngle;
        float lastTurretAngle;
        sf::Clock stateOfPlayerClock;
        sf::Clock updateStateClock;
        sf::Clock drawClock;
        sf::Clock loopClock;
        sf::Clock inputClock;
        sf::Clock accumulatingClock;

        sf::Time previousTime;
        sf::Time currentTime;
        sf::Time deltaTime;
        sf::Time frameTime;
        sf::Clock clock;

        sf::Clock clock2;
    sf::Time frameTime2;
    sf::Time previousTime2;
    sf::Time currentTime2;
    float avgFrameTime;
    std::list<float> times;

        std::vector<sf::Vector2f> posTrack;
        
        bool hasRxStateOfUnion;
        bool hasFocus;

        bool showShadow;
       

        std::list<Explosion> explosions;

        sf::Sprite arenaSprite1;
        sf::RenderTexture arenaTexture1;

        sf::Sprite arenaSprite2;
        sf::RenderTexture arenaTexture2;

        sf::Sprite arenaSprite3;
        sf::RenderTexture arenaTexture3;

        sf::Sprite arenaSprite4;
        sf::RenderTexture arenaTexture4;

        sf::Sprite entitiesSprite;
        sf::RenderTexture entitiesTexture;
        
        sf::Sprite dashSprite;
        sf::RenderTexture dashTexture;


        std::vector<sf::Sprite> floor;
        bool firstRun;

        std::vector<TextureSubInfo> entities;
        std::vector<TextureSubInfo> explosionsBig;
        std::vector<TextureSubInfo> explosionsSmall;

        sf::Text statusOverlay;
        std::vector<sf::Vector2f> obstructionList;
        sf::VertexArray floorVertices;
        sf::VertexArray entityVertices;
        sf::VertexArray explosionSmallVertices;
        sf::VertexArray explosionBigVertices;

        sf::Clock updateFloorClock;
    };
};


#endif

















