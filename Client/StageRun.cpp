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
    hasRxStateOfUnion = false;
    hasFocus = false;
    zoom = 1.0;
}

sf::Uint32 StageRun::doInit()
{
    dash.load();
    return 0;
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
        hasRxStateOfUnion = true;
        //retrieve info on all teams
        //std::cout << "Got StateOfUnion" << std::endl;
        sf::Uint32 teamSize;
        sf::Uint32 slotNum;
        sf::Uint32 hasHost;
        sf::Uint32 cid;
        for (int t= 1;t < 3;t++){
            cevent.packet >> teamSize;//-----------------team 1 , then 2 
            for (int u = 0;u < teamSize;u++){
                cevent.packet >> slotNum;
                Player & aPlayer = teamMan.getPlayerBySlot(t,slotNum);

                cevent.packet >> aPlayer.hasHost;
                cevent.packet >> aPlayer.tank.health;
                cevent.packet >> aPlayer.tank.power;
                cevent.packet >> aPlayer.tank.throttle;
                cevent.packet >> aPlayer.tank.bodyAngle;
                cevent.packet >> aPlayer.tank.turretAngle;
                cevent.packet >> aPlayer.tank.position.x;
                cevent.packet >> aPlayer.tank.position.y;
                cevent.packet >> aPlayer.tank.velocity.x;
                cevent.packet >> aPlayer.tank.velocity.y;
                sf::Uint32 ps;
                cevent.packet >> ps;
                aPlayer.prjctls.clear();
                for (int p = 0;p < ps;p++)
                {
                    Projectile projectile;
                    cevent.packet >> projectile.position.x;
                    cevent.packet >> projectile.position.y;
                    /*cevent.packet >> projectile.velocity.x;
                    cevent.packet >> projectile.velocity.y;
                    cevent.packet >> projectile.angle;*/
                
                    aPlayer.prjctls.push_back(projectile);
                }
            }
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
    }else if (event.type == sf::Event::MouseWheelMoved){
        if (event.mouseWheel.delta  > 0)
            zoom += 0.75;
        else
            zoom -= 0.25;
    }
    //we don't have to check for Resized here, because composing object will 
    //provide updated sizes through setInput.
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
    
    dash.setDash(p);

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
    //We poll keyboard 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        thisPlayer.tank.throttle += 5;
        if (thisPlayer.tank.throttle > 25)
            thisPlayer.tank.throttle = 25;  
        control = true;
    }
    

    //max speed lower in reverse
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        thisPlayer.tank.throttle += -5;
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

        //mouse location is relative to arenaView!
        mouseCursorWorld = window.convertCoords(mouseCursorScreen,arenaView);
        dx = mouseCursorWorld.x - centerOfTurretWorld.x;
        dy = mouseCursorWorld.y - centerOfTurretWorld.y;
        
        lastTurretAngle = curTurretAngle;
        curTurretAngle = (180.0f/3.14156f)*atan2(dy,dx);
        thisPlayer.tank.turretAngle = curTurretAngle;

        control = true;
    }
    
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        attacking = true;
        //thisPlayer.tank.
        std::cout << "Bang!" << std::endl;
    }

    if (control || attacking)
    {
        if (stateOfPlayerClock.getElapsedTime().asMilliseconds() > 50)
        {
            teamMan.teams[myTeam].players[mySlot].state = PlayerState::SendingStateOfPlayer;
            stateOfPlayerClock.restart();
        }
    }

    return 0;
}

sf::Uint32 StageRun::doDraw(sf::RenderWindow & window, TeamManager & teamMan, ArenaManager & arenaMan, AssetManager & assetMan, sf::Time ft)
{
    //TODO: i don't like how you can get invalid CID, Team, and Slot
    sf::Uint32 scrWidth = getInput(0);
    sf::Uint32 scrHeight = getInput(1);
    sf::Uint32 myCID = getInput(2);
    sf::Uint32 myTeam = getInput(3);
    sf::Uint32 mySlot = getInput(4);

   //Set view on top of this player.
    if ((int)myTeam != -1){//if == -1, then your team has not yet been established. i do not like this
        sf::Vector2f pos = teamMan.teams[myTeam].players[mySlot].tank.position;
        //For every 3 world pixels, there is 1 screen pixel
        
        arenaView.reset(sf::FloatRect(pos.x-((scrWidth)/2),pos.y-((scrHeight)/2),scrWidth,scrHeight));
        arenaView.zoom(zoom);
        //arenaView.setViewport(sf::FloatRect(0,0,0.25,0.25));
        window.setView(arenaView);
    }
    //Draw the floor tiles
    for (int i = 0;i < 900;i++){
        Tile &tile = arenaMan.getTile(i);
        sf::Sprite ts;
        ts.setTexture(*assetMan.getFloorImage(tile.getName()).tex);
        ts.setPosition(tile.getPosition());
        window.draw(ts);
    }

    sf::Uint32 t1=0;
    sf::Uint32 t2=0;
     for (int t=1;t < 3;t++)
    {
        for (int s = 0;s < teamMan.teams[t].players.size();s++)
        {
            Player &p = teamMan.teams[t].players[s];
          
            if (p.hasHost)
                if (t==1)
                    t1++;
                else
                    t2++;
        }
     }
    dash.setScore(t1, t2);

    //Draw the tanks
    for (int y=1;y < 3;y++)
    {
        for (int h = 0;h < teamMan.teams[y].players.size();h++)
        {
            if (teamMan.teams[y].players[h].hasHost)
            {
                std::string tankName;
                sf::Sprite b,t;
                //TODO: uh.. i do not like this
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
                for (int k = 0;k < teamMan.teams[y].players[h].prjctls.size();k++)
                {
                    sf::Sprite prjctl;
                    prjctl.setTexture(*assetMan.getProjectileImage("Projectile").tex);
                    prjctl.setPosition(teamMan.teams[y].players[h].prjctls[k].position);
                    //prjctl.setRotation(teamMan.teams[y].players[h].prjctls[k].angle);
                    window.draw(prjctl);
                }

                window.draw(b);
                window.draw(t);
            }
        }
    }

    //Draw the Dashboard
    dashView.reset(sf::FloatRect(0,0, scrWidth, scrHeight));
    //dashView.setCenter(scrWidth/2,-1.0f*(scrHeight/2)+65);
   
    window.setView(dashView);
    
    dash.dashPos.x = 0;
    dash.dashPos.y = scrHeight-65;//65 is height of dashboard backdrop image.

    dash.backDrop.setTexture(*assetMan.getDashboardImage().tex);
    dash.backDrop.setPosition(dash.dashPos);
    
    dash.healthText.setFont(assetMan.getFont());
    dash.healthText.setScale(dash.healthTextScale);
    dash.healthText.setPosition(dash.dashPos.x, dash.dashPos.y+8);
    
    dash.speedText.setFont(assetMan.getFont());
    dash.speedText.setScale(dash.speedTextScale);
    dash.speedText.setPosition(dash.dashPos.x, dash.dashPos.y+33);

    dash.powerText.setFont(assetMan.getFont());
    dash.powerText.setScale(dash.powerTextScale);
    dash.powerText.setPosition(dash.dashPos.x+135, dash.dashPos.y+8);

    dash.scoreText.setFont(assetMan.getFont());
    dash.scoreText.setScale(dash.scoreTextScale);
    dash.scoreText.setPosition(dash.dashPos.x+scrWidth/2,(dash.dashPos.y-scrHeight)+65);

    window.draw(dash.backDrop);
    window.draw(dash.healthText);
    window.draw(dash.speedText);
    window.draw(dash.powerText);
    window.draw(dash.scoreText);

    return 0;
}








