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
    firstRun = true;
    showRunMenu = false;
}

sf::Uint32 StageRun::doInit(Game & g)
{
    //Load HUD
    dash.load();

    //Construct SFGUI menus for later.
    sfg::Button::Ptr quitButton = sfg::Button::Create("Quit?");
    quitButton->GetSignal( sfg::Widget::OnLeftClick ).Connect( &StageRun::doQuit, this );


    mywindow = sfg::Window::Create();    
    mywindow->SetTitle("Mega Blaster Runtime Options");
    mywindow->SetPosition(sf::Vector2f(200.0f,100.0f));
    mywindow->SetRequisition(sf::Vector2f(200,50));
    mywindow->Add(quitButton);

    g.desk.Add(mywindow);
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
                    thisPlayer.tank.throttle = aPlayer.tank.throttle;
                    thisPlayer.tank.bodyAngle= aPlayer.tank.bodyAngle;
                    thisPlayer.tank.velocity = aPlayer.tank.velocity;
                    thisPlayer.tank.health = aPlayer.tank.health;
                    thisPlayer.tank.power = aPlayer.tank.power;
                    
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
            for  (int gi = 0;gi < g.arenaMan.getGeneratorCount(t);gi++)
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

        sf::Uint32 exp_cnt;
        cevent.packet >> exp_cnt;
        //explosions.clear();
        for (int ei = 0;ei < exp_cnt;ei++)
        {
            Explosion ex;
            cevent.packet >> ex.position.x;
            cevent.packet >> ex.position.y;
            cevent.packet >> ex.type;
            ex.index=0;
            explosions.push_back(ex);
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
        else{
            zoom -= 0.5;
            if (zoom < 1)
                zoom=1;

        }
    }

    g.desk.HandleEvent(event);
    
    return 0;
}
sf::Uint32 StageRun::doLoop(Game & g)
{
    if (loopClock.getElapsedTime().asSeconds() > 0.050f)
    {
        if (showRunMenu){
            mywindow->Show(true);
        }
        g.desk.Update(loopClock.getElapsedTime().asSeconds());
        previousTime = currentTime;
        currentTime = clock.restart();
        deltaTime = currentTime - previousTime;
        frameTime += deltaTime;
        Player & p = g.teamMan.getPlayerBySlot(g.myTeam,g.mySlot);

        switch (p.state){
            case PlayerState::Ready:
                p.state = PlayerState::Running;
                break;
            case PlayerState::SendingStateOfPlayer:
                Messages::sendStateOfPlayer(g.client, g.teamMan, g.myCID, g.myTeam, g.mySlot, thisPlayer, attacking);
                obstructionList = prepareObstructionList(g.arenaMan);
                p.state = PlayerState::Running;
            case PlayerState::Running:
                fireProjectile(thisPlayer, accumulatingClock);
                updateVelocity(thisPlayer, obstructionList, frameTime);
                updateProjectilCollisions(g,thisPlayer, g.myTeam, obstructionList, true, frameTime, accumulatingClock);
                break;
        }
        loopClock.restart();
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return getSummary(0).a;
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, Game & g)
{
    if (inputClock.getElapsedTime().asSeconds() > 0.100f)
    {
        if (!hasFocus)
            return 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            showShadow=!showShadow;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            showRunMenu=!showRunMenu;
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

        dash.setDash(thisPlayer);
        g.teamMan.teams[g.myTeam].players[g.mySlot].state = PlayerState::SendingStateOfPlayer;
        
        inputClock.restart();
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return 0;
}

#define STD_ROTATE_OFFSET 90.0f
#define X_CUT_OFF 1792
#define Y_CUT_OFF 1792

sf::Uint32 StageRun::doDraw(sf::RenderWindow & window, Game & g, sf::Time ft)
{

    if (firstRun)
    {
        prepareAssets(g);
        firstRun = false;
    }

    if (drawClock.getElapsedTime().asSeconds() > 0.020f)
    {
        window.clear();
        drawAll(window, g);
        window.display();

        if (showRunMenu){
            window.resetGLStates();
            g.sfGui.Display(window);
        }

        
        drawClock.restart();
    }else{
        sf::sleep(sf::seconds(0.0f));
    }
    return 0;
}

sf::Uint32 StageRun::prepareAssets(Game &g)
{
    arenaTexture.create(g.arenaMan.getMapHorizTileNum()*125,g.arenaMan.getMapVertTileNum()*125);
    entitiesTexture.create(g.arenaMan.getMapHorizTileNum()*125,g.arenaMan.getMapVertTileNum()*125);
    dashTexture.create(512,65);

    arenaTexture.clear();
    //arenaTexture.setSmooth(true);
    for (int i = 0;i < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum();i++){
        Tile &tile = g.arenaMan.getTile(i);
            
        /*if (abs(tile.getPosition().x - pos.x) < X_CUT_OFF &&
            abs(tile.getPosition().y - pos.y) < Y_CUT_OFF )
        {*/
            sf::Sprite s = g.assetMan.getSprite(tile.getId()+4);
            s.setPosition(tile.getPosition());
            
            arenaTexture.draw(s);
        /*}*/
    }
    arenaTexture.display();
    arenaSprite.setTexture(arenaTexture.getTexture());

    statusOverlay.setFont(g.assetMan.getFont());
    statusOverlay.setScale(2.0,2.0);
    statusOverlay.setColor(sf::Color::Green);
    return 0;
}



sf::Uint32 StageRun::drawAll(sf::RenderWindow & window, Game & g)
{
    //Set view on top of this player...smoothly.
    //TODO: move calculations into doLoop
    sf::Vector2f pos = thisPlayer.tank.position;
    posTrack.push_back(pos);
    if (posTrack.size() > 30)
        posTrack.erase(posTrack.begin());
    pos = sf::Vector2f(0,0);
    for (auto m = 0;m < posTrack.size();m++)
        pos += posTrack[m];
    pos.x  = pos.x / 30.0f;
    pos.y  = pos.y / 30.0f;

    //TODO: assumes more wide than tall screen.
    float aspect = (float)g.scrHeight / (float)g.scrWidth;
    sf::FloatRect fr = sf::FloatRect(pos.x-((g.scrWidth)/2.0f),pos.y-((g.scrHeight)/2.0f),g.scrWidth*aspect,g.scrHeight*aspect);
    
    arenaView.reset(fr);
    arenaView.zoom(zoom);
    window.setView(arenaView);
       
    window.draw(arenaSprite);
    entitiesTexture.clear(sf::Color(0,0,0,0));
        
       
    //Draw the tanks
    for (int y=1;y < 3;y++)
    {
        for (int h = 0;h < g.teamMan.teams[y].players.size();h++)
        {
            if (g.teamMan.teams[y].players[h].hasHost)
            {
                if (g.myTeam == y && g.mySlot == h)
                {
                    //Draw My Locally Predicted Tank
                    sf::Sprite b = g.assetMan.getSprite((y==1 ? ImageType::TankBlue : ImageType::TankRed));//g.assetMan.getTankImage((y==1 ? 0 : 1)).bsprite;
                    sf::Sprite t = g.assetMan.getSprite((y==1 ? ImageType::TurretBlue : ImageType::TurretRed));//g.assetMan.getTankImage((y==1 ? 0 : 1)).tsprite;

                    b.setOrigin(37,58);
                    t.setOrigin(24,40);

                    b.setRotation(thisPlayer.tank.bodyAngle-STD_ROTATE_OFFSET);
                    t.setRotation(thisPlayer.tank.turretAngle-STD_ROTATE_OFFSET);

                    b.setPosition(thisPlayer.tank.position);
                    t.setPosition(thisPlayer.tank.position);

                    entitiesTexture.draw(b);
                    entitiesTexture.draw(t);
                    
                    //Draw Projectiles
                    for (int k = 0;k < thisPlayer.prjctls.size();k++)
                    {
                        sf::Sprite p = g.assetMan.getSprite(ImageType::Projectile1);//.getImage(ImageType::Projectile1).sprite;
                        p.setOrigin(16.0f,16.0f);
                        p.setPosition(thisPlayer.prjctls[k].position);
                        entitiesTexture.draw(p);
                    }

                }else{

                    //Draw other players' tank with last server update

                   
                    sf::Sprite b = g.assetMan.getSprite((y==1 ? ImageType::TankBlue : ImageType::TankRed));
                    sf::Sprite t = g.assetMan.getSprite((y==1 ? ImageType::TurretBlue : ImageType::TurretRed));

                    b.setOrigin(37,58);
                    t.setOrigin(24,40);

                    b.setRotation(g.teamMan.teams[y].players[h].tank.bodyAngle-STD_ROTATE_OFFSET);
                    t.setRotation(g.teamMan.teams[y].players[h].tank.turretAngle-STD_ROTATE_OFFSET);

                    b.setPosition(g.teamMan.teams[y].players[h].tank.position);
                    t.setPosition(g.teamMan.teams[y].players[h].tank.position);

                    entitiesTexture.draw(b);
                    entitiesTexture.draw(t);

                    for (int k = 0;k < g.teamMan.teams[y].players[h].prjctls.size();k++)
                    {
                        sf::Sprite p = g.assetMan.getSprite(ImageType::Projectile1);//.getImage(ImageType::Projectile1).sprite;
                        p.setOrigin(16.0f,16.0f);
                        p.setPosition(g.teamMan.teams[y].players[h].prjctls[k].position);
                        entitiesTexture.draw(p);
                    }
                }
            }
        }
    }
    
    //Draw Creep
    {
        for (int y = 1;y < 3;y++)
        {
            sf::Sprite creep = g.assetMan.getSprite( (y==1 ? ImageType::Minion1 : ImageType::Minion2));
            creep.setOrigin(16.0f,16.0f);
            for (int lk = 0;lk < g.teamMan.teams[y].creep.size();lk++)
            {
                /*if (abs(g.teamMan.teams[y].creep[lk].position.x - pos.x) < X_CUT_OFF &&
                    abs(g.teamMan.teams[y].creep[lk].position.y - pos.y) < Y_CUT_OFF )
                {*/
               
                    creep.setRotation(g.teamMan.teams[y].creep[lk].angle);
                    creep.setPosition(g.teamMan.teams[y].creep[lk].position);
                    entitiesTexture.draw(creep);
                /*}*/
            }
        }
    }

    //Draw Generator lasers
    {
        sf::Sprite prjctl = g.assetMan.getSprite(ImageType::ProjectileDeathRay);
        prjctl.setOrigin(32.0f,32.0f);
        for (int y = 1;y < 3;y++)
        {
            for (int gi = 0;gi < g.arenaMan.getGeneratorCount(y);gi++)
            {
                for (int k = 0;k < g.teamMan.teams[y].gen[gi].prjctls.size();k++)
                {
                
                    prjctl.setPosition(g.teamMan.teams[y].gen[gi].prjctls[k].position);
                    prjctl.setRotation(g.teamMan.teams[y].gen[gi].prjctls[k].angle);
                    entitiesTexture.draw(prjctl);
                }
            }
        }
    }

    //Draw Base heal ray
    {
        sf::Sprite prjctl = g.assetMan.getSprite(ImageType::ProjectileHealRay);
        prjctl.setOrigin(32.0f,32.0f);
        for (int y = 1;y < 3;y++)
        {
            for (int k = 0;k < g.teamMan.teams[y].base1.prjctls.size();k++)
            {
            
                prjctl.setPosition(g.teamMan.teams[y].base1.prjctls[k].position);
                prjctl.setRotation(g.teamMan.teams[y].base1.prjctls[k].angle);
                entitiesTexture.draw(prjctl);
            }
        }
    }

    //Draw the Dashboard
    
    
    dashTexture.clear(sf::Color(0,0,0,0));

    dash.dashPos.x = 0;
    dash.dashPos.y = 0;//g.scrHeight-130;//65 is height of dashboard backdrop image.

    sf::Sprite bd = g.assetMan.getSprite(ImageType::Dash1);//dash.backDrop.setTexture(g.assetMan.getImage(ImageType::Dash1).tex);
    bd.setPosition(dash.dashPos);
    
    dash.healthText.setFont(g.assetMan.getFont());
    dash.healthText.setScale(dash.healthTextScale);
    dash.healthText.setPosition(dash.dashPos.x, dash.dashPos.y+8);
    
    dash.speedText.setFont(g.assetMan.getFont());
    dash.speedText.setScale(dash.speedTextScale);
    dash.speedText.setPosition(dash.dashPos.x, dash.dashPos.y+33);

    dash.powerText.setFont(g.assetMan.getFont());
    dash.powerText.setScale(dash.powerTextScale);
    dash.powerText.setPosition(dash.dashPos.x+135, dash.dashPos.y+8);


    dashTexture.draw(bd);
    dashTexture.draw(dash.healthText);
    dashTexture.draw(dash.speedText);
    dashTexture.draw(dash.powerText);
        
    dashTexture.display();
    dashSprite.setTexture(dashTexture.getTexture());

    

    //Draw Explosions!!
    {
        for (auto e = explosions.begin();e != explosions.end();){
            sf::Sprite exp;
            if (e->type == ExplosionType::TankHit      ||
                e->type == ExplosionType::CreepHit     ||
                e->type == ExplosionType::GeneratorHit ||
                e->type == ExplosionType::BaseHit)
            {
                exp = g.assetMan.getSprite(ImageType::Explosion2);//.setTexture(g.assetMan.getImage(ImageType::Explosion2).tex);
            }else{
                exp = g.assetMan.getSprite(ImageType::Explosion1);
            }
            
            exp.setScale(4.0f,4.0f);
            int xi,yi;

            xi = (e->index % 4) * 64;
            yi = (e->index / 4) * 64;
            exp.setTextureRect(sf::IntRect(xi,yi,64,64));
            exp.setOrigin(32,32);
            exp.setPosition(e->position);

            entitiesTexture.draw(exp);

            bool done = false;
            if (e->rate.getElapsedTime().asMilliseconds() > 75){
                //std::cout << xi << ", " << yi << std::endl;
                e->index = (e->index + 1) % 20;
                if (e->index == 19){
                    done = true;
                }
                e->rate.restart();
            }
        
            if (done){
                e = explosions.erase(e);
            }else
                e++;
        }
    }

    //Draw  status overlays
    for (int y=1;y < 3;y++)
    {
        for (int h = 0;h < g.teamMan.teams[y].players.size();h++)
        {
            if (g.teamMan.teams[y].players[h].hasHost)
            {
                std::stringstream ss;
                if (g.myTeam == y && g.mySlot == h)
                {
                    ss << thisPlayer.tank.health ;
                    statusOverlay.setString(ss.str());
                    statusOverlay.setPosition(thisPlayer.tank.position.x-55,thisPlayer.tank.position.y-120);
                    entitiesTexture.draw(statusOverlay);
                }else{
                    //Draw Health Overlay
                    ss << g.teamMan.teams[y].players[h].tank.health ;
                    statusOverlay.setString(ss.str());
                    statusOverlay.setPosition(g.teamMan.teams[y].players[h].tank.position.x-55,g.teamMan.teams[y].players[h].tank.position.y-120);
                    entitiesTexture.draw(statusOverlay);
                }
            }
        }
    }
    entitiesSprite.setTexture(entitiesTexture.getTexture());
    entitiesTexture.display();        
    window.draw(entitiesSprite);
    
    dashView.reset(sf::FloatRect(0,0, g.scrWidth, g.scrHeight));   
    window.setView(dashView);//set view to dash mode..
    window.draw(dashSprite);
    window.setView(arenaView);//Set view back to normal mode.
    ////MiniMap
    //sf::View miniMap;
    //miniMap.reset(sf::FloatRect(0,0, g.scrWidth, g.scrHeight));
    //miniMap.zoom(zoom);
    //miniMap.setViewport(sf::FloatRect(0.8,0.8, 0.2,0.2));
    //window.setView(miniMap);
    //window.draw(entitiesSprite);
    ////window.draw(arenaSprite);

    return 0;
}

void StageRun::doQuit()
{
    //Tell Game that this stage is finished
    Element e0;
    e0.a = 1;
    setSummary(e0,0);

    //Tell game why this stage is finished - user quit
    Element e1;
    e1.a = 1;
    setSummary(e1,1);
}

