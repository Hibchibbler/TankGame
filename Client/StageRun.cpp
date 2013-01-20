#include "StageRun.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "Common\AssetManager.h"
#include "Common\LogFile.h"
#include "Common\Physics.h"

#include <sstream>
using namespace tg;

StageRun::StageRun(Game & g)
    : GameStage(g)
{
    hasRxStateOfUnion = false;
    hasFocus = false;
    zoom = 1.0;
    showShadow = false;
}

sf::Uint32 StageRun::doInit()
{
    dash.load();
    return 0;
}

sf::Uint32 StageRun::doRemoteEvent(Game & g,
                            CommEvent & cevent,
                            sf::Uint32 connId,
                            sf::Uint32 msgId)
{
    Player &player = g.teamMan.getPlayer(connId);
    switch (msgId){
    case MsgId::StateOfUnion:
        
        //retrieve info on all teams
        //std::cout << "Got StateOfUnion" << std::endl;
        sf::Uint32 teamSize;
        sf::Uint32 slotNum;

        for (int t= 1;t < 3;t++){
            //Players
            cevent.packet >> teamSize;//-----------------team 1 , then 2 
            for (int u = 0;u < teamSize;u++){
                cevent.packet >> slotNum;
                Player & aPlayer = g.teamMan.getPlayerBySlot(t,slotNum);
                
                cevent.packet >> aPlayer.hasHost;
                cevent.packet >> aPlayer.tank.health;
                cevent.packet >> aPlayer.tank.power;
                cevent.packet >> aPlayer.tank.throttle;
                cevent.packet >> aPlayer.tank.bodyAngle;
                cevent.packet >> aPlayer.tank.turretAngle;
                cevent.packet >> aPlayer.tank.shadowPos.x;
                cevent.packet >> aPlayer.tank.shadowPos.y;
                cevent.packet >> aPlayer.tank.velocity.x;
                cevent.packet >> aPlayer.tank.velocity.y;

                
                aPlayer.tank.shadowUpdated = true;
                if (!hasRxStateOfUnion)
                {
                    aPlayer.tank.position = aPlayer.tank.shadowPos;
                }

                //SNAP into place when an update is gotten
                if (t == g.myTeam &&
                    u == g.mySlot)
                {
                    //std::cout << "(" << thisPlayer.tank.position.x << ", " << thisPlayer.tank.position.y << "), (" << aPlayer.tank.shadowPos.x << ", " << aPlayer.tank.shadowPos.y << ")" << std::endl;
                    
                    sf::Vector2f d;
                    d = aPlayer.tank.shadowPos - thisPlayer.tank.position;
                    thisPlayer.tank.position += d/3.0f;

                    //float dba = (aPlayer.tank.bodyAngle - thisPlayer.tank.bodyAngle);
                    //std::cout << aPlayer.tank.bodyAngle << ", " << thisPlayer.tank.bodyAngle << std::endl;
                    thisPlayer.tank.bodyAngle=aPlayer.tank.bodyAngle;// += dba;
                }else
                {
                    aPlayer.tank.position = aPlayer.tank.shadowPos;
                }
                 
                sf::Uint32 ps;
                cevent.packet >> ps;
                aPlayer.prjctls.clear();
                for (int p = 0;p < ps;p++)
                {
                    Projectile projectile;
                    cevent.packet >> projectile.position.x;
                    cevent.packet >> projectile.position.y;
                    aPlayer.prjctls.push_back(projectile);
                }
            }
            //Teams
            //  Creep
            sf::Uint32 cc;
            g.teamMan.teams[t].creep.clear();
            cevent.packet >> cc;
            for (int c = 0 ;c < cc ;c++)
            {
                Creep creep;
                cevent.packet >> creep.position.x;
                cevent.packet >> creep.position.y;
                cevent.packet >> creep.angle;
                g.teamMan.teams[t].creep.push_back(creep);
            }

            //  Generator Death Laser
            for  (int gi = 0;gi < 2;gi++)
            {
                g.teamMan.teams[t].gen[gi].prjctls.clear();
                cevent.packet >> cc;
                for (int b = 0;b < cc;b++)
                {
                    Projectile proj;
                    cevent.packet >> proj.position.x;
                    cevent.packet >> proj.position.y;
                    cevent.packet >> proj.angle;
                    g.teamMan.teams[t].gen[gi].prjctls.push_back(proj);
                }
            }
            //  Base Heal Laser
            g.teamMan.teams[t].base1.prjctls.clear();
            cevent.packet >> cc;
            for (int b = 0;b < cc;b++)
            {
                Projectile proj;
                cevent.packet >> proj.position.x;
                cevent.packet >> proj.position.y;
                cevent.packet >> proj.angle;
                g.teamMan.teams[t].base1.prjctls.push_back(proj);
            }
        }
        hasRxStateOfUnion = true;
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
            zoom += 0.5;
        else
            zoom -= 0.5;
    }
    return 0;
}
sf::Uint32 StageRun::doLoop(Game & g)
{
    if (loopClock.getElapsedTime().asMilliseconds() > 10)
    {
        previousTime = currentTime;
        currentTime = clock.restart();
        deltaTime = currentTime - previousTime;
        frameTime += deltaTime;
        Player & p = g.teamMan.getPlayerBySlot(g.myTeam,g.mySlot);
    
        dash.setDash(p);

        switch (p.state){
            case PlayerState::Ready:
                p.state = PlayerState::Running;
                break;
            case PlayerState::SendingStateOfPlayer:
                Messages::sendStateOfPlayer(g.client, g.teamMan, g.myCID, g.myTeam, g.mySlot, thisPlayer, attacking);
                p.state = PlayerState::Running;
                
            case PlayerState::Running:
                MeAndMyTank___Bitch(g,thisPlayer, g.myTeam,frameTime, accumulatingClock);
                break;

        }
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return getSummary(0).a;
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, Game & g)
{
    if (inputClock.getElapsedTime().asSeconds() > 0.030f)
    {
        if (!hasFocus)
            return 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            showShadow=!showShadow;
        }
        //thisPlayer = g.teamMan.teams[g.myTeam].players[g.mySlot];
        //We poll keyboard 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            thisPlayer.tank.accelerating = AccelerateAction::Forward;
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            thisPlayer.tank.accelerating = AccelerateAction::Reverse;
        }else
        {
            //doLoop will nullify this action after sending
            //it to the server.
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            //Rotating left
            thisPlayer.tank.turningBody = TurnAction::Left;
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            //Rotating right
            thisPlayer.tank.turningBody = TurnAction::Right;
        }else
        {
           //doLoop will nullify this action after sending
           //it to the server.
        }
    
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            thisPlayer.tank.attacking = AttackAction::Attacking;
        }else{
          //doLoop will nullify this action after sending
          //it to the server.
        }

        lastMousePos = curMousePos;
        curMousePos = sf::Mouse::getPosition(window);
        if (curMousePos != lastMousePos || thisPlayer.tank.attacking == AttackAction::Attacking)
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
        }

      
        g.teamMan.teams[g.myTeam].players[g.mySlot].state = PlayerState::SendingStateOfPlayer;
        
        inputClock.restart();
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return 0;
}


sf::Uint32 StageRun::doDraw(sf::RenderWindow & window, Game & g, sf::Time ft)
{
    if (drawClock.getElapsedTime().asSeconds() > 0.020f)
    {
        window.clear();
        //Set view on top of this player.
        if ((int)g.myTeam != -1){//if == -1, then your team has not yet been established. TODO: i do not like this
            //TODO: move calculations into doLoop
            sf::Vector2f pos = thisPlayer.tank.position;
            posTrack.push_back(pos);
            if (posTrack.size() > 30)
                posTrack.erase(posTrack.begin());

            pos = sf::Vector2f(0,0);
            for (auto m = 0;m < posTrack.size();m++){
                pos += posTrack[m];
            }
            pos.x  = pos.x / 30.0f;
            pos.y  = pos.y / 30.0f;

            arenaView.reset(sf::FloatRect(pos.x-((g.scrWidth)/2.0f),pos.y-((g.scrHeight)/2.0f),g.scrWidth,g.scrHeight));//sf::FloatRect(0,0,g.scrWidth,g.scrHeight));//
            arenaView.zoom(zoom);
            window.setView(arenaView);
        }
        //Draw the floor tiles
        for (int i = 0;i < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum();i++){
            Tile &tile = g.arenaMan.getTile(i);
            sf::Sprite ts;
            ts.setTexture(*g.assetMan.getFloorImage(tile.getName()).tex);
            ts.setPosition(tile.getPosition());
            window.draw(ts);
        }

        sf::Uint32 t1=0;
        sf::Uint32 t2=0;
            for (int t=1;t < 3;t++)
        {
            for (int s = 0;s < g.teamMan.teams[t].players.size();s++)
            {
                Player &p = g.teamMan.teams[t].players[s];
          
                if (p.hasHost && p.tank.health > 0)
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
            for (int h = 0;h < g.teamMan.teams[y].players.size();h++)
            {
                if (g.teamMan.teams[y].players[h].hasHost)
                {
                    std::string tankName;
                    sf::Sprite b,t;
                    
                    //Team 1 is blue, team 2 is red.
                    if (y==1)
                        tankName = "BlueTank";
                    else if (y==2)
                        tankName = "RedTank";

                    TankImage & ti = g.assetMan.getTankImage(tankName);
                    TankImage & sti = g.assetMan.getTankImage("ShadowTank");
                    
                    if (g.myTeam == y && g.mySlot == h)
                    {
                        //Draw My Locally Predicted Tank
                        b.setTexture(*ti.btex);
                        t.setTexture(*ti.ttex);

                        b.setOrigin(40,61);
                        t.setOrigin(27,90);

                        b.setRotation(-90);
                        t.setRotation(-90);

                        b.rotate(thisPlayer.tank.bodyAngle);
                        t.rotate(thisPlayer.tank.turretAngle);

                        b.setPosition(thisPlayer.tank.position);
                        t.setPosition(thisPlayer.tank.position);

                        window.draw(b);
                        window.draw(t);

                        //Draw Projectiles
                        for (int k = 0;k < thisPlayer.prjctls.size();k++)
                        {
                            sf::Sprite prjctl;
                            prjctl.setTexture(*g.assetMan.getProjectileImage("Projectile").tex);
                            prjctl.setOrigin(16.0f,16.0f);
                            prjctl.setPosition(thisPlayer.prjctls[k].position);
                            window.draw(prjctl);
                        }

                        //Draw Health Overlay
                        sf::Text stat;
                        std::stringstream ss;
                        //ss << y << "/" << g.teamMan.teams[y].players[h].tank.health << "/" << g.teamMan.teams[y].players[h].tank.power;
                        ss << g.teamMan.teams[y].players[h].tank.health ;
                        stat.setString(ss.str());
                        stat.setFont(g.assetMan.getFont());
                        stat.setScale(2.0,2.0);
                        stat.setColor(sf::Color::Green);
                        stat.setPosition(thisPlayer.tank.position.x-55,thisPlayer.tank.position.y-120);
                        window.draw(stat);

                        if (showShadow)
                        {
                            //Draw my Tank with last server update
                            b.setTexture(*sti.btex);
                            t.setTexture(*sti.ttex);

                            b.setOrigin(40,61);
                            t.setOrigin(27,90);

                            b.setRotation(-90);
                            t.setRotation(-90);

                            b.rotate(g.teamMan.teams[y].players[h].tank.bodyAngle);
                            t.rotate(g.teamMan.teams[y].players[h].tank.turretAngle);

                            b.setPosition(g.teamMan.teams[y].players[h].tank.shadowPos);
                            t.setPosition(g.teamMan.teams[y].players[h].tank.shadowPos);

                            window.draw(b);
                            window.draw(t);

                             for (int k = 0;k < g.teamMan.teams[y].players[h].prjctls.size();k++)
                            {
                                sf::Sprite prjctl;
                                prjctl.setTexture(*g.assetMan.getProjectileImage("ProjectileShadow").tex);
                                prjctl.setOrigin(8.0f,8.0f);
                                prjctl.setPosition(g.teamMan.teams[y].players[h].prjctls[k].position);
                                window.draw(prjctl);
                            }
                        }
                        
                    }else{

                        //Draw other players' tank with last server update

                        b.setTexture(*ti.btex);
                        t.setTexture(*ti.ttex);

                        b.setOrigin(40,61);
                        t.setOrigin(27,90);

                        b.setRotation(-90);
                        t.setRotation(-90);

                        b.rotate(g.teamMan.teams[y].players[h].tank.bodyAngle);
                        t.rotate(g.teamMan.teams[y].players[h].tank.turretAngle);

                        b.setPosition(g.teamMan.teams[y].players[h].tank.position);
                        t.setPosition(g.teamMan.teams[y].players[h].tank.position);

                        window.draw(b);
                        window.draw(t);

                        //Draw Health Overlay
                        sf::Text stat;
                        std::stringstream ss;
                        //ss << y << "/" << g.teamMan.teams[y].players[h].tank.health << "/" << g.teamMan.teams[y].players[h].tank.power;
                        ss << g.teamMan.teams[y].players[h].tank.health ;
                        stat.setString(ss.str());
                        stat.setFont(g.assetMan.getFont());
                        stat.setScale(2.0,2.0);
                        stat.setColor(sf::Color::Green);
                        stat.setPosition(g.teamMan.teams[y].players[h].tank.position.x-55,g.teamMan.teams[y].players[h].tank.position.y-120);
                        window.draw(stat);

                        for (int k = 0;k < g.teamMan.teams[y].players[h].prjctls.size();k++)
                        {
                            sf::Sprite prjctl;
                            prjctl.setTexture(*g.assetMan.getProjectileImage("Projectile").tex);
                            prjctl.setOrigin(16.0f,16.0f);
                            prjctl.setPosition(g.teamMan.teams[y].players[h].prjctls[k].position);
                            window.draw(prjctl);
                        }
                    }

                    if (g.teamMan.teams[y].players[h].tank.health == 0){
                        
                        sf::Sprite exp;
                        exp.setTexture(*g.assetMan.getTankDeathExplosionImage().tex);
                        exp.setScale(4.0f,4.0f);
                        int xi,yi;

                        xi = (g.teamMan.teams[y].players[h].tank.explosionIndex % 4) * 64;
                        yi = (g.teamMan.teams[y].players[h].tank.explosionIndex / 4) * 64;
                        exp.setTextureRect(sf::IntRect(xi,yi,64,64));
                        exp.setOrigin(32,32);
                        exp.setPosition(g.teamMan.teams[y].players[h].tank.shadowPos);

                        window.draw(exp);

                        if (g.teamMan.teams[y].players[h].tank.explosionClock.getElapsedTime().asMilliseconds() > 75){
                            //std::cout << xi << ", " << yi << std::endl;
                            g.teamMan.teams[y].players[h].tank.explosionIndex = (g.teamMan.teams[y].players[h].tank.explosionIndex + 1) % 20;
                            g.teamMan.teams[y].players[h].tank.explosionClock.restart();
                        }
                    }

                    
                }
            
                //Draw Creep
                for (int lk = 0;lk < g.teamMan.teams[y].creep.size();lk++)
                {
                    sf::Sprite creep;
                    creep.setTexture(*g.assetMan.getMinionImage((y==1 ?"Minion1":"Minion2")).tex);
                    creep.setScale(1.5,1.5);
                    creep.setPosition(g.teamMan.teams[y].creep[lk].position);

                    creep.setRotation(g.teamMan.teams[y].creep[lk].angle);
                    window.draw(creep);
                }
            
                //Draw Generator lasers
                for (int gi = 0;gi < 2;gi++)
                {
                    for (int k = 0;k < g.teamMan.teams[y].gen[gi].prjctls.size();k++)
                    {
                        sf::Sprite prjctl;
                        prjctl.setTexture(*g.assetMan.getProjectileImage("BaseLaser").tex);
                        prjctl.setOrigin(16.0f,16.0f);
                        prjctl.setPosition(g.teamMan.teams[y].gen[gi].prjctls[k].position);
                        prjctl.setRotation(g.teamMan.teams[y].gen[gi].prjctls[k].angle);
                        window.draw(prjctl);
                    }
                }
                //Draw Base heal ray
                for (int k = 0;k < g.teamMan.teams[y].base1.prjctls.size();k++)
                {
                    sf::Sprite prjctl;
                    prjctl.setTexture(*g.assetMan.getProjectileImage("HealLaser").tex);
                    prjctl.setOrigin(16.0f,16.0f);
                    prjctl.setPosition(g.teamMan.teams[y].base1.prjctls[k].position);
                    prjctl.setRotation(g.teamMan.teams[y].base1.prjctls[k].angle);
                    window.draw(prjctl);
                }
            }
        }

        //Draw the Dashboard
        dashView.reset(sf::FloatRect(0,0, g.scrWidth, g.scrHeight));
   
        window.setView(dashView);
    
        dash.dashPos.x = 0;
        dash.dashPos.y = g.scrHeight-65;//65 is height of dashboard backdrop image.

        dash.backDrop.setTexture(*g.assetMan.getDashboardImage().tex);
        dash.backDrop.setPosition(dash.dashPos);
    
        dash.healthText.setFont(g.assetMan.getFont());
        dash.healthText.setScale(dash.healthTextScale);
        dash.healthText.setPosition(dash.dashPos.x, dash.dashPos.y+8);
    
        dash.speedText.setFont(g.assetMan.getFont());
        dash.speedText.setScale(dash.speedTextScale);
        dash.speedText.setPosition(dash.dashPos.x, dash.dashPos.y+33);

        dash.powerText.setFont(g.assetMan.getFont());
        dash.powerText.setScale(dash.powerTextScale);
        dash.powerText.setPosition(dash.dashPos.x+135, dash.dashPos.y+8);

        dash.scoreText.setFont(g.assetMan.getFont());
        dash.scoreText.setScale(dash.scoreTextScale);
        dash.scoreText.setPosition(dash.dashPos.x+g.scrWidth/2,(dash.dashPos.y-g.scrHeight)+65);

        window.draw(dash.backDrop);
        window.draw(dash.healthText);
        window.draw(dash.speedText);
        window.draw(dash.powerText);
        window.draw(dash.scoreText);

        window.display();

        drawClock.restart();
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return 0;
}








