#include "StageRun.h"
#include "Common\Game.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "Common\AssetManager.h"
#include <sstream>
using namespace tg;

StageRun::StageRun(Game & g)
    : GameStage(g)
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

                bool auth=false;
                for (int x = 0;x < aPlayer.tank.posHistory.size();x++)
                {
                    //if (g.teamMan.teams[y].players[h].tank.posHistory[x].
                    if (aPlayer.tank.posHistory[x].pos ==  aPlayer.tank.shadowPos)
                        auth = true;
                }

                if (!auth)
                {
                    aPlayer.tank.position = aPlayer.tank.shadowPos;
                    aPlayer.tank.posHistory.clear();
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
    //we don't have to check for Resized here, because composing object will 
    //provide updated sizes through setInput.
    return 0;
}

sf::Uint32 StageRun::doLoop(Game & g)
{
 
    if (loopClock.getElapsedTime().asSeconds() > 0.020f)
    {
        Player & p = g.teamMan.getPlayerBySlot(g.myTeam,g.mySlot);
    
        dash.setDash(p);

        switch (p.state){
            case PlayerState::Ready:
                p.state = PlayerState::Running;
                break;
            case PlayerState::SendingStateOfPlayer:
                Messages::sendStateOfPlayer(g.client, g.teamMan, g.myCID, g.myTeam, g.mySlot, thisPlayer, attacking);
                p.state = PlayerState::Running;
                break;
            case PlayerState::Running:

                break;

        }
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return getSummary(0).a;
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, Game & g)
{
    if (inputClock.getElapsedTime().asSeconds() > 0.100f)
    {
        //static bool prevControl = false;
        bool curControl = false;
        attacking = 0;
    
        if (!hasFocus)
            return 0;

    

        thisPlayer = g.teamMan.teams[g.myTeam].players[g.mySlot];
        //We poll keyboard 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            thisPlayer.tank.throttle += 2;
            if (thisPlayer.tank.throttle > 56)
                thisPlayer.tank.throttle = 56;  
            curControl = true;
        }
    

        //max speed lower in reverse
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            thisPlayer.tank.throttle += -2;
            if (thisPlayer.tank.throttle < -8)
                thisPlayer.tank.throttle = -8;
            curControl = true;
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            //Rotating left

            thisPlayer.tank.bodyAngle = thisPlayer.tank.bodyAngle -  15;
            if (thisPlayer.tank.bodyAngle >= 360.0f || thisPlayer.tank.bodyAngle <= -360.0f ){
                thisPlayer.tank.bodyAngle = 0;
            }
            curControl = true;
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            //Rotating right
            thisPlayer.tank.bodyAngle = thisPlayer.tank.bodyAngle +  15;
            if (thisPlayer.tank.bodyAngle >= 360.0f || thisPlayer.tank.bodyAngle <= -360.0f ){
                thisPlayer.tank.bodyAngle = 0;
            }
            curControl = true;
        }
    
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            attacking = true;
            //std::cout << "Bang!" << std::endl;
        }

        lastMousePos = curMousePos;
        curMousePos = sf::Mouse::getPosition(window);
        if (curMousePos != lastMousePos || attacking)
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

            curControl = true;
        }

        if (curControl)
        {
            thisPlayer.tank.velocity.x = thisPlayer.tank.throttle * (float)cos(thisPlayer.tank.bodyAngle * (0.0174531f));
            thisPlayer.tank.velocity.y = thisPlayer.tank.throttle * (float)sin(thisPlayer.tank.bodyAngle * (0.0174531f));
        }
       // if (stateOfPlayerClock.getElapsedTime().asMilliseconds() > 100)
       // {
            g.teamMan.teams[g.myTeam].players[g.mySlot].state = PlayerState::SendingStateOfPlayer;
       //   stateOfPlayerClock.restart();
       // }
        inputClock.restart();
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return 0;
}
#define LINEAR_SMOOTH 20.0f
static std::vector<sf::Vector2f> posTrack;
sf::Uint32 StageRun::doDraw(sf::RenderWindow & window, Game & g, sf::Time ft)
{
    if (drawClock.getElapsedTime().asSeconds() > 0.020f)
    {
        previousTime = currentTime;
        currentTime = clock.restart();
        deltaTime = currentTime - previousTime;
        frameTime += deltaTime;

        window.clear();
        //Set view on top of this player.
        if ((int)g.myTeam != -1){//if == -1, then your team has not yet been established. TODO: i do not like this
            //TODO: move calculations into doLoop
            sf::Vector2f pos = g.teamMan.teams[g.myTeam].players[g.mySlot].tank.position;
            posTrack.push_back(pos);
            if (posTrack.size() > 70)
                posTrack.erase(posTrack.begin());

            pos = sf::Vector2f(0,0);
            for (auto m = 0;m < posTrack.size();m++){
                pos += posTrack[m];
            }
            pos.x  = pos.x / 70.0f;
            pos.y  = pos.y / 70.0f;

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
                    if (g.myTeam == y && g.mySlot == h)
                    {
                        //std::cout << frameTime.asSeconds() << std::endl;
                        g.teamMan.teams[y].players[h].tank.position.x += thisPlayer.tank.velocity.x * frameTime.asSeconds() * 10.0f;
                        g.teamMan.teams[y].players[h].tank.position.y += thisPlayer.tank.velocity.y * frameTime.asSeconds() * 10.0f;

                        AuthPos ap;
                        ap.pos = g.teamMan.teams[y].players[h].tank.position;
                        ap.authorized = false;
                        g.teamMan.teams[y].players[h].tank.posHistory.push_back(ap);
                        if (g.teamMan.teams[y].players[h].tank.posHistory.size() > 15)
                            g.teamMan.teams[y].players[h].tank.posHistory.erase(g.teamMan.teams[y].players[h].tank.posHistory.begin());

                    }else
                    {
                        g.teamMan.teams[y].players[h].tank.position = g.teamMan.teams[y].players[h].tank.shadowPos;
                    }

                    
                

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

                    TankImage & ti = g.assetMan.getTankImage(tankName);

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

                    for (int k = 0;k < g.teamMan.teams[y].players[h].prjctls.size();k++)
                    {
                        sf::Sprite prjctl;
                        prjctl.setTexture(*g.assetMan.getProjectileImage("Projectile").tex);
                        prjctl.setOrigin(8.0f,8.0f);
                        prjctl.setPosition(g.teamMan.teams[y].players[h].prjctls[k].position);
                        window.draw(prjctl);
                    }

                    window.draw(b);
                    window.draw(t);

                    sf::Text stat;
                    std::stringstream ss;
                    ss << y << "/" << g.teamMan.teams[y].players[h].tank.health << "/" << g.teamMan.teams[y].players[h].tank.power;
                    stat.setString(ss.str());
                    stat.setFont(g.assetMan.getFont());
                    stat.setScale(2.0,2.0);
                    stat.setColor(sf::Color::Green);
                    stat.setPosition(g.teamMan.teams[y].players[h].tank.position.x-75,g.teamMan.teams[y].players[h].tank.position.y-120);
                    window.draw(stat);
                }
            
                //Draw Creep
                for (int lk = 0;lk < g.teamMan.teams[y].creep.size();lk++)
                {
                    sf::Sprite creep;
                    creep.setTexture(*g.assetMan.getMinionImage((y==1 ?"Minion1":"Minion2")).tex);
                    creep.setScale(1.5,1.5);
                    creep.setPosition(g.teamMan.teams[y].creep[lk].position);
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








