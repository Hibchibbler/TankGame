#include "StageRun.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "Common\AssetManager.h"

using namespace tg;

StageRun::StageRun()
    : GameStage()
{
    hasFocus = false;
}

sf::Uint32 StageRun::doRemoteEvent(TeamManager & teamMan, 
                            ArenaManager & arenaMan,
                            CommEvent & cevent,
                            sf::Uint32 connId,
                            sf::Uint32 msgId)
{
    Player &player = teamMan.getPlayer(connId);
    switch (msgId){
    case MsgId::StateOfUnion:
        //retrieve info on all teams
        //std::cout << "Got StateOfUnion" << std::endl;
        sf::Uint32 teamSize;
        sf::Uint32 slotNum;
        sf::Uint32 hasHost;
        sf::Uint32 cid;

        cevent.packet >> teamSize;//-----------------team 1  
        for (int u = 0;u < teamSize;u++){
            cevent.packet >> slotNum;
            Player & aPlayer = teamMan.getPlayerBySlot(1,slotNum);

            cevent.packet >> aPlayer.hasHost;
            cevent.packet >> aPlayer.tank.bodyAngle;
            cevent.packet >> aPlayer.tank.turretAngle;
            cevent.packet >> aPlayer.tank.position.x;
            cevent.packet >> aPlayer.tank.position.y;
            cevent.packet >> aPlayer.tank.velocity.x;
            cevent.packet >> aPlayer.tank.velocity.y;


        }


        cevent.packet >> teamSize;//-----------------team 2  
        for (int u = 0;u < teamSize;u++){
            cevent.packet >> slotNum;
            Player & aPlayer = teamMan.getPlayerBySlot(2,slotNum);

            cevent.packet >> aPlayer.hasHost;
            cevent.packet >> aPlayer.tank.bodyAngle;
            cevent.packet >> aPlayer.tank.turretAngle;
            cevent.packet >> aPlayer.tank.position.x;
            cevent.packet >> aPlayer.tank.position.y;
            cevent.packet >> aPlayer.tank.velocity.x;
            cevent.packet >> aPlayer.tank.velocity.y;
        }

        break;
    }
    return 0;
}

sf::Uint32 StageRun::doWindowEvent(sf::RenderWindow & w, 
                                          sf::Event & event)
{
    if (event.type == sf::Event::LostFocus){
        hasFocus = false;
    }else if (event.type == sf::Event::GainedFocus){
        hasFocus = true;
    }

    //sf::View aView;
    //aView.setCenter(0,0);
    //aView.setSize(600,600);
    //w.setView(aView);
    return 0;
}

sf::Uint32 StageRun::doLoop(Comm & comm, TeamManager & teamMan)
{
    sf::Uint32 scrWidth  = getInput(0);
    sf::Uint32 scrHeight = getInput(1);
    sf::Uint32 myCID     = getInput(2);
    sf::Uint32 myTeam    = getInput(3);
    sf::Uint32 mySlot    = getInput(4);


    
    Player & p = teamMan.getPlayerBySlot(myTeam,mySlot);
    switch (p.state){
        case PlayerState::Ready:
            p.state = PlayerState::Running;
            break;
        case PlayerState::SendingStateOfPlayer:
            Messages::sendStateOfPlayer(comm, teamMan, myCID, myTeam, mySlot, thisPlayer, attacking);
            p.state = PlayerState::Running;
            break;

    }
    return getSummary(0);
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, TeamManager & teamMan)
{
    sf::Uint32 scrWidth = getInput(0);
    sf::Uint32 scrHeight = getInput(1);
    sf::Uint32 myCID = getInput(2);
    sf::Uint32 myTeam = getInput(3);
    sf::Uint32 mySlot = getInput(4);


    bool control = false;
    attacking = 0;
    
    if (!hasFocus)
        return 0;
    thisPlayer = teamMan.teams[myTeam].players[mySlot];
    //We poll keyboard instead of relying on bios keyboard rate
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        thisPlayer.tank.throttle += 1;
        if (thisPlayer.tank.throttle > 25)
            thisPlayer.tank.throttle = 25;  
        control = true;
    }
    

    //max speed lower in reverse
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        thisPlayer.tank.throttle -= 1;
        if (thisPlayer.tank.throttle < -25)
            thisPlayer.tank.throttle = -25;  
        control = true;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        //Rotating left

        thisPlayer.tank.bodyAngle = thisPlayer.tank.bodyAngle -  5;
        if (thisPlayer.tank.bodyAngle >= 360.0f || thisPlayer.tank.bodyAngle <= -360.0f ){
            thisPlayer.tank.bodyAngle = 0;
        }
        control = true;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        //Rotating right
        thisPlayer.tank.bodyAngle = thisPlayer.tank.bodyAngle +  5;
        if (thisPlayer.tank.bodyAngle >= 360.0f || thisPlayer.tank.bodyAngle <= -360.0f ){
            thisPlayer.tank.bodyAngle = 0;
        }
        control = true;
    }
    
    lastMousePos = curMousePos;
    curMousePos = sf::Mouse::getPosition(window);
    if (curMousePos != lastMousePos)
    {
        float dx,dy;
        sf::Vector2f centerOfTurretWorld;
        sf::Vector2i mouseCursorScreen(curMousePos.x,curMousePos.y);
        sf::Vector2f mouseCursorWorld;

        //Find turrent angle based on vector from tank origin to mouse.
        centerOfTurretWorld = thisPlayer.tank.position;
        mouseCursorWorld = window.convertCoords(mouseCursorScreen);
        dx = mouseCursorWorld.x - centerOfTurretWorld.x;
        dy = mouseCursorWorld.y - centerOfTurretWorld.y;
        
        lastTurretAngle = curTurretAngle;
        curTurretAngle = (180.0f/3.14156f)*atan2(dy,dx);
        thisPlayer.tank.turretAngle = curTurretAngle;

        control = true;
    }
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        attacking = true;
        std::cout << "Bang!" << std::endl;
    }

    //TODO: 
    if (stateOfPlayerClock.getElapsedTime().asMilliseconds() > 100 ||
        control || attacking){
        
        teamMan.teams[myTeam].players[mySlot].state = PlayerState::SendingStateOfPlayer;
        stateOfPlayerClock.restart();
    }
    return 0;
}

sf::Uint32 StageRun::doDraw(sf::RenderWindow & window, TeamManager & teamMan, AssetManager & assetMan, sf::Time ft)
{
    sf::View v(sf::FloatRect(-300,-300,600,600));
    window.setView(v);
    for (int y=1;y < 3;y++)
    {
        for (int h = 0;h < teamMan.teams[y].players.size();h++)
        {
            if (teamMan.teams[y].players[h].hasHost)
            {
                std::string tankName;
                sf::Sprite b,t;
                //TODO: uh..
                if (y==1 && h == 0)
                    tankName = "BlueTank";
                else if (y==1 && h==1)
                    tankName = "RedTank";
                else if (y==2 && h==0)
                    tankName = "GreenTank";
                else
                    tankName = "YellowTank";

                TankImage & ti = assetMan.getTankImage(tankName);
            
                b.setTexture(*ti.btex);
                t.setTexture(*ti.ttex);

                b.setOrigin(40,61);
                t.setOrigin(27,90);

                b.setRotation(-90);
                t.setRotation(-90);

                b.rotate(teamMan.teams[y].players[h].tank.bodyAngle);
                t.rotate(teamMan.teams[y].players[h].tank.turretAngle);

                b.setPosition(teamMan.teams[y].players[h].tank.position);
                t.setPosition(teamMan.teams[y].players[h].tank.position);

                /*std::cout << teamMan.teams[y].players[h].tank.position.x << ", " << teamMan.teams[y].players[h].tank.position.y << std::endl;
                std::cout << teamMan.teams[y].players[h].tank.velocity.x*20*ft.asSeconds() << ", " << teamMan.teams[y].players[h].tank.position.y*20*ft.asSeconds() << std::endl;*/
                

                window.draw(b);
                window.draw(t);
            }
        }
    }
    return 0;
}








