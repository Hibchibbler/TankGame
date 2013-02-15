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
    hasFocus = true;
    zoom = 1.0;
    showShadow = false;
    firstRun = true;
    showRunMenu = false;
    zoomPow = 1;
    visionRange = 1000;
    viewPreset = 0;
}

sf::Uint32 StageRun::doInit(Game & g)
{
    //Load HUD
    dash.load();

    //Prepare obstruction list(once)
    obstructionList = prepareObstructionList(g.arenaMan);
    
    //viewRect = sf::FloatRect((int)(g.arenaMan.getStartPosition(g.myTeam).x-(g.scrWidth/2.0f)),(int)(g.arenaMan.getStartPosition(g.myTeam).y-(g.scrHeight/2.0f)),g.scrWidth,g.scrHeight);

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
        sf::Uint32 visible;
        for (int t= 1;t < 3;t++){
            //Players
            cevent.packet >> teamSize;//-----------------team 1 , then 2 
            for (int u = 0;u < teamSize;u++){
                cevent.packet >> slotNum;
                Player & aPlayer = g.teamMan.getPlayerBySlot(t,slotNum);
                
                cevent.packet >> aPlayer.hasHost;
                cevent.packet >>aPlayer.tank.visible;
                if (aPlayer.tank.visible == 1){
                    cevent.packet >> aPlayer.tank.health;
                    cevent.packet >> aPlayer.tank.power;
                    cevent.packet >> aPlayer.tank.throttle;
                    cevent.packet >> aPlayer.tank.bodyAngle;
                    cevent.packet >> aPlayer.tank.turretAngle;
                    cevent.packet >> aPlayer.tank.shadowPos.x;
                    cevent.packet >> aPlayer.tank.shadowPos.y;
                    cevent.packet >> aPlayer.tank.velocity.x;
                    cevent.packet >> aPlayer.tank.velocity.y;
                    
                }
                
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
                cevent.packet >> creep.creepType;
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
                    g.teamMan.teams[t].gen[gi].lastFiringAngle = proj.angle;
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
        {
            /*if (zoom == 1)
                zoom = 3;
            else if (zoom == 3)
                zoom = 5;
            else if (zoom == 5)
                zoom = 7;
            else if (zoom == 7)
                zoom = 9;
            else if (zoom == 9)
                zoom = 11;
            else if (zoom == 11)
                zoom = 13;*/
            zoom +=1;
        }else{
            //if (zoom == 13)
            //    zoom = 11;
            //else if (zoom == 11)
            //    zoom = 9;
            //else if (zoom == 9)
            //    zoom = 7;
            //else if (zoom == 7)
            //    zoom = 5;
            //else if (zoom == 5)
            //    zoom = 3;
            //else if (zoom == 3)
            //    zoom = 1;
            zoom -= 1;

        }
        std::cout << "Zoom: " << zoom << std::endl;
    }else if (event.type == sf::Event::MouseEntered){
        hasFocus = true;
    }else if (event.type == sf::Event::MouseLeft){
        hasFocus = false;
    }else if (event.type == sf::Event::Resized){
        
    }
    
    g.desk.HandleEvent(event);
    
    return 0;
}
sf::Uint32 StageRun::doLoop(Game & g)
{
    if (loopClock.getElapsedTime().asSeconds() > 0.025f)
    {
        g.desk.Update(loopClock.getElapsedTime().asSeconds());
        previousTime = currentTime;
        currentTime = clock.restart();
        deltaTime = currentTime - previousTime;
        frameTime += deltaTime;
        //std::cout << 1.0f/frameTime.asSeconds() << std::endl;
        Player & p = g.teamMan.getPlayerBySlot(g.myTeam,g.mySlot);

        switch (p.state){
            case PlayerState::Ready:
                p.state = PlayerState::Running;
                break;
            case PlayerState::SendingStateOfPlayer:
                Messages::sendStateOfPlayer(g.client, g.teamMan, g.myCID, g.myTeam, g.mySlot, thisPlayer, attacking);
                
                p.state = PlayerState::Running;
            case PlayerState::Running:{
            
                        
                        //sf::Clock c;
                        fireProjectile(thisPlayer, accumulatingClock);
                
                        updateVelocity(thisPlayer, obstructionList, frameTime);
                        //std::cout << c.getElapsedTime().asSeconds() << std::endl;
                        updateProjectilCollisions(g,thisPlayer, g.myTeam, obstructionList, true, frameTime, accumulatingClock);


              
                break;
            }
        }
        loopClock.restart();
    }
    return getSummary(0).a;
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, Game & g)
{
    if (!hasFocus)
        return 0;
    //Window Scrolling 
    if (scrollClock.getElapsedTime().asSeconds() > 0.06)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosWorld = window.convertCoords(mousePos,arenaView);

        //If view shifts, then next time spacebar is hit, snap to tank...
        //hence, viewPreset=0;
        if (mousePosWorld.x > (arenaView.getCenter().x+(g.scrWidth/4)*zoom) ){
            arenaView.move(100.0f, 0);
            viewPreset = 0;
        }else if (mousePosWorld.x < (arenaView.getCenter().x-(g.scrWidth/4)*zoom) ){
            arenaView.move(-100.0f, 0);
            viewPreset = 0;
        }

    
        if (mousePosWorld.y > (arenaView.getCenter().y+(g.scrHeight/4)*zoom) ){
            arenaView.move(0, 100.0f);
            viewPreset = 0;
        }else if (mousePosWorld.y < (arenaView.getCenter().y-(g.scrHeight/4)*zoom) ){
            arenaView.move(0, -100.0f);
            viewPreset = 0;
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            if (viewPreset == 0){
                arenaView.setCenter(thisPlayer.tank.position);
                viewPreset = (viewPreset + 1) % 2;
            }else if (viewPreset == 1)
            {
                arenaView.setCenter(g.arenaMan.getStartPosition(g.myTeam));
                viewPreset = (viewPreset + 1) % 2;
            }
        }

        scrollClock.restart();
    }

    


    if (inputClock.getElapsedTime().asSeconds() > 0.075f)
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            showShadow=!showShadow;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            showRunMenu=!showRunMenu;
            if (showRunMenu){
                mywindow->Show(true);
            }
        }

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
    
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))// && fireClock.getElapsedTime().asSeconds() > 0.150f)
        {
            thisPlayer.tank.attacking = AttackAction::Attacking;
            fireClock.restart();
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

        times.push_back(frameTime2.asSeconds());
        if (times.size() > 100)
            times.erase(times.begin());
        float at=0.0f;
        for (auto k = times.begin();k != times.end();k++){
            at += *k;
        }
        at /= 100.0f;
        dash.setDash(thisPlayer, 1.0f/at);
        

        g.teamMan.teams[g.myTeam].players[g.mySlot].state = PlayerState::SendingStateOfPlayer;
        
        inputClock.restart();
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

    if (true)//drawClock.getElapsedTime().asSeconds() > 0.001f)
    {

        window.clear();
        drawAll(window, g);
       
        if (showRunMenu){
            window.resetGLStates();
            g.sfGui.Display(window);
        }
        
        window.display();

        
        drawClock.restart();
    }
    return 0;
}

sf::Uint32 StageRun::addStraightQuad(sf::VertexArray & v, sf::FloatRect c, sf::IntRect t)
{
    v.append(sf::Vertex(sf::Vector2f(c.left, c.top),
                        sf::Vector2f(t.left, t.top)
                       ));

    v.append(sf::Vertex(sf::Vector2f(c.left+c.width, c.top),
                        sf::Vector2f(t.left+t.width, t.top)
                       ));

    v.append(sf::Vertex(sf::Vector2f(c.left+c.width, c.top+c.height),
                        sf::Vector2f(t.left+t.width, t.top+t.height)
                       ));

    v.append(sf::Vertex(sf::Vector2f(c.left, c.top+c.height),
                        sf::Vector2f(t.left, t.top+t.height)
                       ));

    return 0;
}


sf::Uint32 StageRun::addRotQuad(sf::VertexArray & v, sf::FloatRect p, sf::IntRect t, float angle)
{

    float px1,py1;
    float px2,py2;
    float px3,py3;
    float px4,py4;

    angle = angle*(3.14156f/180.0f);

    px1 = ((0-p.width/2.0f) * cos(angle) - (0-p.height/2.0f) * sin(angle)) + p.left;
    py1 = ((0-p.width/2.0f) * sin(angle) + (0-p.height/2.0f) * cos(angle)) + p.top;

    px2 = ((p.width-p.width/2.0f) * cos(angle) - (0-p.height/2.0f) * sin(angle)) + p.left;
    py2 = ((p.width-p.width/2.0f) * sin(angle) + (0-p.height/2.0f) * cos(angle)) + p.top;

    px3 = ((p.width-p.width/2.0f) * cos(angle) - (p.height-p.height/2.0f) * sin(angle)) + p.left;
    py3 = ((p.width-p.width/2.0f) * sin(angle) + (p.height-p.height/2.0f) * cos(angle)) + p.top;

    px4 = ((0-p.width/2.0f) * cos(angle) - (p.height-p.height/2.0f) * sin(angle)) + p.left;
    py4 = ((0-p.width/2.0f) * sin(angle) + (p.height-p.height/2.0f) * cos(angle)) + p.top;


    v.append(sf::Vertex(sf::Vector2f(px1,py1),
                        sf::Vector2f(t.left, t.top)
                       ));

    v.append(sf::Vertex(sf::Vector2f(px2,py2),
                        sf::Vector2f(t.left+t.width, t.top)
                       ));

    v.append(sf::Vertex(sf::Vector2f(px3,py3),
                        sf::Vector2f(t.left+t.width, t.top+t.height)
                       ));

    v.append(sf::Vertex(sf::Vector2f(px4,py4),
                        sf::Vector2f(t.left, t.top+t.height)
                       ));

    return 0;
}


sf::Uint32 StageRun::prepareAssets(Game &g)
{
    floorVertices = sf::VertexArray(sf::PrimitiveType::Quads);
    //entitiesTexture.create(g.arenaMan.getMapHorizTileNum()*128,g.arenaMan.getMapVertTileNum()*128);
    entityVertices = sf::VertexArray(sf::PrimitiveType::Quads);
    //dashTexture.create(512,65);
    explosionSmallVertices = sf::VertexArray(sf::PrimitiveType::Quads);
    explosionBigVertices = sf::VertexArray(sf::PrimitiveType::Quads);

    statusOverlay.setFont(g.assetMan.getFont());
    statusOverlay.setScale(2.0,2.0);
    statusOverlay.setColor(sf::Color::Green);

    //viewRect = sf::FloatRect((int)(g.arenaMan.getStartPosition(g.myTeam).x-(g.scrWidth/2.0f)),(int)(g.arenaMan.getStartPosition(g.myTeam).y-(g.scrHeight/2.0f)),g.scrWidth,g.scrHeight);
    arenaView.setCenter((int)(g.arenaMan.getStartPosition(g.myTeam).x-(g.scrWidth/2.0f)),
                        (int)(g.arenaMan.getStartPosition(g.myTeam).y-(g.scrHeight/2.0f)));
    return 0;
}

#define BASE_VISION_RANGE 700




sf::Uint32 StageRun::drawAll(sf::RenderWindow & window, Game & g)
{   

    previousTime2 = currentTime2;
    currentTime2 = clock2.restart();
    frameTime2 += currentTime2 - previousTime2;
    //std::cout << 1.0f/frameTime2.asSeconds() << std::endl;

    sf::Vector2f pos = thisPlayer.tank.position;

    arenaView.setSize(g.scrWidth,g.scrHeight);
    arenaView.zoom(zoom);
    window.setView(arenaView);
    
    pos = thisPlayer.tank.position;

    int myTeam = g.myTeam;
    int otherTeam = (g.myTeam == 1 ? 2 : 1);

    partFogQuick(g.arenaMan.getStartPosition(g.myTeam), g, fogMask2, 225);

    entityVertices.clear();
    //Arena Entities - bases, turrets, totems, waypoints
    addStraightQuad(entityVertices,
                    sf::FloatRect(g.arenaMan.getStartPosition(g.myTeam).x, g.arenaMan.getStartPosition(g.myTeam).y, 128,128),
                    g.assetMan.getSprite(ImageType::Base1).getTextureRect());

    
    addStraightQuad(entityVertices,
                sf::FloatRect(g.arenaMan.getStartPosition(otherTeam).x, g.arenaMan.getStartPosition(otherTeam).y, 128,128),
                g.assetMan.getSprite(ImageType::Base2).getTextureRect());
    
    


    //My Tanks
    sf::Uint32 curTeam = g.myTeam;
    sf::Sprite blueBody = g.assetMan.getSprite((curTeam==1 ? ImageType::TankBlue : ImageType::TankRed));
    sf::Sprite blueTurret = g.assetMan.getSprite((curTeam==1 ? ImageType::TurretBlue : ImageType::TurretRed));

    for (int h = 0;h < g.teamMan.teams[curTeam].players.size();h++)
    {
        if (g.teamMan.teams[curTeam].players[h].hasHost &&
            g.teamMan.teams[curTeam].players[h].tank.health > 0)
        {
            sf::Vector2f curPos;
            if (g.mySlot == h )
            {
                //Draw me
                curPos = thisPlayer.tank.position;
                addRotQuad(entityVertices,
                            sf::FloatRect(thisPlayer.tank.position.x, thisPlayer.tank.position.y, 73,116),
                            blueBody.getTextureRect(),
                            thisPlayer.tank.bodyAngle-STD_ROTATE_OFFSET);

                addRotQuad(entityVertices,
                            sf::FloatRect(thisPlayer.tank.position.x,thisPlayer.tank.position.y, 47,176),
                            blueTurret.getTextureRect(),
                            thisPlayer.tank.turretAngle-STD_ROTATE_OFFSET);
            }else
            {
                //Draw my team
                curPos = g.teamMan.teams[curTeam].players[h].tank.position;
                addRotQuad(entityVertices,
                                sf::FloatRect(g.teamMan.teams[curTeam].players[h].tank.position.x, g.teamMan.teams[curTeam].players[h].tank.position.y, 73,116),
                                blueBody.getTextureRect(),
                                g.teamMan.teams[curTeam].players[h].tank.bodyAngle-STD_ROTATE_OFFSET);

                addRotQuad(entityVertices,
                                sf::FloatRect(g.teamMan.teams[curTeam].players[h].tank.position.x, g.teamMan.teams[curTeam].players[h].tank.position.y, 47,176),
                                blueTurret.getTextureRect(),
                                g.teamMan.teams[curTeam].players[h].tank.turretAngle-STD_ROTATE_OFFSET);
        
            }

            //We ain't dead, so be visible.
            g.teamMan.teams[curTeam].players[h].tank.visible = 1;

            //Clear fog for my team tanks
            partFogQuick(curPos, g, fogMask2, 225);
            //partFog(curPos,g);
        }else
        {
            g.teamMan.teams[curTeam].players[h].tank.visible = 0;
            if (g.mySlot == h )
            {
                addRotQuad(entityVertices,
                            sf::FloatRect(thisPlayer.tank.position.x, thisPlayer.tank.position.y, 64,64),
                            g.assetMan.getSprite(ImageType::Ghost).getTextureRect(),
                            thisPlayer.tank.bodyAngle-STD_ROTATE_OFFSET);
            }
        }
    }

     //My Teams projectiles 
    sf::Sprite p = g.assetMan.getSprite(ImageType::Projectile1);
    for (int h = 0;h < g.teamMan.teams[curTeam].players.size();h++)
    {
        if (g.teamMan.teams[curTeam].players[h].hasHost)
        {
            if (g.mySlot == h)
            {
                //My Projectiles
                for (int k = 0;k < thisPlayer.prjctls.size();k++)
                {
                    addRotQuad(entityVertices,
                                sf::FloatRect(thisPlayer.prjctls[k].position.x, thisPlayer.prjctls[k].position.y, 32,32),
                                p.getTextureRect(),
                                0.0f);
                }
            }else
            {
                //My teammates
                for (int k = 0;k < g.teamMan.teams[curTeam].players[h].prjctls.size();k++)
                {
                    addRotQuad(entityVertices,
                                sf::FloatRect(g.teamMan.teams[curTeam].players[h].prjctls[k].position.x, g.teamMan.teams[curTeam].players[h].prjctls[k].position.y, 32,32),
                                p.getTextureRect(),
                                0.0f);
                }
            }
        }
    }

    //My Creep
    //sf::Sprite creep = g.assetMan.getSprite( (curTeam==1 ? ImageType::Minion1 : ImageType::Minion2));
    for (int lk = 0;lk < g.teamMan.teams[curTeam].creep.size();lk++)
    {
        sf::Sprite creep = g.assetMan.getSprite(g.teamMan.teams[curTeam].creep[lk].creepType);
        addRotQuad(entityVertices,
                    sf::FloatRect(g.teamMan.teams[curTeam].creep[lk].position.x, 
                                    g.teamMan.teams[curTeam].creep[lk].position.y, 64,64),
                    creep.getTextureRect(),
                    g.teamMan.teams[curTeam].creep[lk].angle);
            
        //Clear fog for my team minion
        partFogQuick(g.teamMan.teams[curTeam].creep[lk].position, g, fogMask0, 121);
    }

    //Opponent Tanks
    curTeam = (curTeam == 1 ? 2 : 1);
    blueBody = g.assetMan.getSprite((curTeam==1 ? ImageType::TankBlue : ImageType::TankRed));
    blueTurret = g.assetMan.getSprite((curTeam==1 ? ImageType::TurretBlue : ImageType::TurretRed));

    for (int h = 0;h < g.teamMan.teams[curTeam].players.size();h++)
    {
        if (g.teamMan.teams[curTeam].players[h].hasHost &&
            g.teamMan.teams[curTeam].players[h].tank.health > 0 &&
            g.teamMan.teams[curTeam].players[h].tank.visible == 1)
        {
            /*int index;
            g.arenaMan.posToIndex(g.teamMan.teams[curTeam].players[h].tank.position, index);
            if (index >= 0 && index < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum() && !g.arenaMan.getTile(index).fog)
            {*/
                //Draw their tanks
                addRotQuad(entityVertices,
                                sf::FloatRect(g.teamMan.teams[curTeam].players[h].tank.position.x, g.teamMan.teams[curTeam].players[h].tank.position.y, 73,116),
                                blueBody.getTextureRect(),
                                g.teamMan.teams[curTeam].players[h].tank.bodyAngle-STD_ROTATE_OFFSET);

                addRotQuad(entityVertices,
                                sf::FloatRect(g.teamMan.teams[curTeam].players[h].tank.position.x, g.teamMan.teams[curTeam].players[h].tank.position.y, 47,176),
                                blueTurret.getTextureRect(),
                                g.teamMan.teams[curTeam].players[h].tank.turretAngle-STD_ROTATE_OFFSET);
                //g.teamMan.teams[curTeam].players[h].tank.visible = true;
            //}else{
            //    //Opponent tank is under fog - don't draw
            //    g.teamMan.teams[curTeam].players[h].tank.visible = false;
            //}
        //}else
        //{
        //    //Opponent tank has no health - don't draw
        //    g.teamMan.teams[curTeam].players[h].tank.visible = false;
        }
    }

    //Draw other teams projectiles
    for (int h = 0;h < g.teamMan.teams[curTeam].players.size();h++)
    {
        if (g.teamMan.teams[curTeam].players[h].hasHost)
        {
            for (int k = 0;k < g.teamMan.teams[curTeam].players[h].prjctls.size();k++)
            {
                /*int index;
                g.arenaMan.posToIndex(g.teamMan.teams[curTeam].players[h].prjctls[k].position, index);
                if (index >= 0 && index < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum() && !g.arenaMan.getTile(index).fog)
                {*/
                    addRotQuad(entityVertices,
                                sf::FloatRect(g.teamMan.teams[curTeam].players[h].prjctls[k].position.x, g.teamMan.teams[curTeam].players[h].prjctls[k].position.y, 32,32),
                                p.getTextureRect(),
                                0.0f);
                /*}*/
            }
        }
    }
    
    //Their Creep
    //creep = g.assetMan.getSprite( (curTeam==1 ? ImageType::Minion1 : ImageType::Minion2));
    for (int lk = 0;lk < g.teamMan.teams[curTeam].creep.size();lk++)
    {
        sf::Sprite creep = g.assetMan.getSprite(g.teamMan.teams[curTeam].creep[lk].creepType);
        /*int index;
        g.arenaMan.posToIndex(g.teamMan.teams[curTeam].creep[lk].position, index);
        if (index >= 0 && index < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum() && !g.arenaMan.getTile(index).fog)
        {*/

            addRotQuad(entityVertices,
                    sf::FloatRect(g.teamMan.teams[curTeam].creep[lk].position.x, 
                                    g.teamMan.teams[curTeam].creep[lk].position.y, 64,64),
                    creep.getTextureRect(),
                    g.teamMan.teams[curTeam].creep[lk].angle);
        /*}*/
    }

    //Draw Generator lasers    
    sf::Sprite prjctl = g.assetMan.getSprite(ImageType::ProjectileDeathRay);
    prjctl.setOrigin(32.0f,32.0f);
    
    for (int y = 1;y < 3;y++)
    {
        for (int gi = 0;gi < g.arenaMan.getGeneratorCount(y);gi++)
        {
            for (int k = 0;k < g.teamMan.teams[y].gen[gi].prjctls.size();k++)
            {
               /* float dx,dy;
                dx = g.teamMan.teams[y].gen[gi].prjctls[k].position.x - pos.x;
                dy = g.teamMan.teams[y].gen[gi].prjctls[k].position.y - pos.y;

                if (sqrt(dx*dx+dy*dy) < visionRange)
                {*/
                    addRotQuad(entityVertices,
                                sf::FloatRect(g.teamMan.teams[y].gen[gi].prjctls[k].position.x, g.teamMan.teams[y].gen[gi].prjctls[k].position.y, 64,64),
                                prjctl.getTextureRect(),
                                g.teamMan.teams[y].gen[gi].prjctls[k].angle);
               /* }*/
            }
        }
    }

    //Draw turrets after drawing lazer..so it looks like it is coming out of the barrel....
    for (int gc = 0;gc < g.arenaMan.getGeneratorCount(myTeam);gc++)
    {
        addRotQuad(entityVertices,
                        sf::FloatRect(g.arenaMan.getGeneratorPosition(myTeam, gc).x, g.arenaMan.getGeneratorPosition(myTeam,gc).y, 128,128),
                        g.assetMan.getSprite(ImageType::Generator1).getTextureRect(),
                        g.teamMan.teams[myTeam].gen[gc].lastFiringAngle+180.0f);
    }
    for (int gc = 0;gc < g.arenaMan.getGeneratorCount(otherTeam);gc++)
    {
        addRotQuad(entityVertices,
                        sf::FloatRect(g.arenaMan.getGeneratorPosition(otherTeam, gc).x, g.arenaMan.getGeneratorPosition(otherTeam,gc).y, 128,128),
                        g.assetMan.getSprite(ImageType::Generator2).getTextureRect(),
                        g.teamMan.teams[otherTeam].gen[gc].lastFiringAngle+180.0f);
    }

    //Draw Base heal ray
    prjctl = g.assetMan.getSprite(ImageType::ProjectileHealRay);
    for (int y = 1;y < 3;y++)
    {
        for (int k = 0;k < g.teamMan.teams[y].base1.prjctls.size();k++)
        {
            /*float dx,dy;
            dx = g.teamMan.teams[y].base1.prjctls[k].position.x - pos.x;
            dy = g.teamMan.teams[y].base1.prjctls[k].position.y - pos.y;

            if (sqrt(dx*dx+dy*dy) < visionRange)
            {*/
                addRotQuad(entityVertices,
                            sf::FloatRect(g.teamMan.teams[y].base1.prjctls[k].position.x, g.teamMan.teams[y].base1.prjctls[k].position.y, 64,64),
                            prjctl.getTextureRect(),
                            g.teamMan.teams[y].base1.prjctls[k].angle);
          /*  }*/
        }
    }
    
    //Draw the floor and fog
    if (updateFloorClock.getElapsedTime().asMilliseconds() > 100)
    {
        sf::Sprite fog15 = g.assetMan.getSprite(ImageType::FogOfWar15);
        sf::Sprite fog30 = g.assetMan.getSprite(ImageType::FogOfWar30);
        sf::Sprite fog45 = g.assetMan.getSprite(ImageType::FogOfWar45);
        sf::Sprite fog60 = g.assetMan.getSprite(ImageType::FogOfWar60);
        sf::Sprite fog75 = g.assetMan.getSprite(ImageType::FogOfWar75);

        floorVertices.clear();
        for (int i = 0;i < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum();i++){
            Tile &tile = g.arenaMan.getTile(i);
            sf::Sprite s = g.assetMan.getSprite(tile.getId());

            if (abs(tile.getPosition().x - window.getView().getCenter().x) < g.scrWidth*zoom &&
                abs(tile.getPosition().y - window.getView().getCenter().y) < g.scrHeight*zoom)
            {
                addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), s.getTextureRect());

                int index = tile.fogFalseIndex;// tile.fog ?  tile.fogTrueIndex :  tile.fogFalseIndex;
                switch (index){
                case 0:
                    //No Fog
                    break;
                case 1:
                    addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), fog15.getTextureRect());
                    break;
                case 2:
                    addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), fog30.getTextureRect());
                    break;
                case 3:
                    addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), fog45.getTextureRect());
                    break;
                case 4:
                    addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), fog60.getTextureRect());
                    break;
                case 5:
                    addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), fog75.getTextureRect());
                    break;
                }
      
               if (tile.fogClock.getElapsedTime().asSeconds() > 0.1f){
                    tile.fog = true;
                    tile.fogFalseIndex++;
                    if (tile.fogFalseIndex > 5){
                        tile.fogFalseIndex = 5;
                       
                    }

                    tile.fogClock.restart();
                }
            }
        }
        updateFloorClock.restart();
    }
    
    

    //Draw Hit Explosions!!
    explosionSmallVertices.clear();
    for (auto e = explosions.begin();e != explosions.end();){
        sf::Sprite exp;

        if (e->type == ExplosionType::TankHit      ||
            e->type == ExplosionType::CreepHit     ||
            e->type == ExplosionType::GeneratorHit ||
            e->type == ExplosionType::BaseHit)
        {
            
            exp = g.assetMan.getSprite(ImageType::ExplosionSmall);
            int xi,yi;

            xi = (e->index % 4) * 64;
            yi = (e->index / 4) * 64;
           
            /*float dx,dy;
            dx = e->position.x-128 - pos.x;
            dy = e->position.y-128 - pos.y;

            if (sqrt(dx*dx+dy*dy) < visionRange)
            {*/

                addStraightQuad(explosionSmallVertices,
                            sf::FloatRect(e->position.x-128, e->position.y-128, 256,256),
                            sf::IntRect(xi,yi,64,64));
            /*}*/
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
            continue;
        }
        e++;
    }
    

    explosionBigVertices.clear();
    for (auto e = explosions.begin();e != explosions.end();){
        sf::Sprite exp;

        if (e->type == ExplosionType::TankDeath      ||
            e->type == ExplosionType::CreepDeath     ||
            e->type == ExplosionType::GeneratorDeath ||
            e->type == ExplosionType::BaseDeath)
        {
            exp = g.assetMan.getSprite(ImageType::ExplosionBig);
            int xi,yi;

            xi = (e->index % 4) * 64;
            yi = (e->index / 4) * 64;
            
            /*float dx,dy;
            dx = e->position.x-128 - pos.x;
            dy = e->position.y-128 - pos.y;

            if (sqrt(dx*dx+dy*dy) < visionRange)
            {*/
                addStraightQuad(explosionBigVertices,
                            sf::FloatRect(e->position.x-128, e->position.y-128, 256,256),
                            sf::IntRect(xi,yi,64,64));
            /*}*/
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
            continue;
        }
        e++;
    }

    window.draw(floorVertices, &g.assetMan.getTexture(0));
    window.draw(entityVertices, &g.assetMan.getTexture(0));
    window.draw(explosionSmallVertices, &g.assetMan.getTexture(1));
    window.draw(explosionBigVertices, &g.assetMan.getTexture(2));

    //Draw  status overlays
    for (int y=1;y < 3;y++)
    {
        for (int h = 0;h < g.teamMan.teams[y].players.size();h++)
        {
            if (g.teamMan.teams[y].players[h].hasHost &&
                g.teamMan.teams[y].players[h].tank.visible==1)
            {
                std::stringstream ss;
                if (g.myTeam == y && g.mySlot == h)
                {
                    ss << thisPlayer.tank.health ;
                    statusOverlay.setString(ss.str());
                    statusOverlay.setPosition(thisPlayer.tank.position.x-55,thisPlayer.tank.position.y-120);
                    window.draw(statusOverlay);
                }else{
                    //Draw Health Overlay
                    ss << g.teamMan.teams[y].players[h].tank.health ;
                    statusOverlay.setString(ss.str());
                    statusOverlay.setPosition(g.teamMan.teams[y].players[h].tank.position.x-55,g.teamMan.teams[y].players[h].tank.position.y-120);
                    window.draw(statusOverlay);
                }
            }
        }
    }
    
    //Draw the Dashboard
    dash.dashPos.x = 0.0f;
    dash.dashPos.y = 0.0f;//g.scrHeight-130;//65 is height of dashboard backdrop image.

    dash.healthText.setFont(g.assetMan.getFont());
    dash.healthText.setScale(dash.healthTextScale);
    dash.healthText.setPosition(dash.dashPos.x, dash.dashPos.y+8.0f);
    
    dash.speedText.setFont(g.assetMan.getFont());
    dash.speedText.setScale(dash.speedTextScale);
    dash.speedText.setPosition(dash.dashPos.x, dash.dashPos.y+33.0f);

    dash.powerText.setFont(g.assetMan.getFont());
    dash.powerText.setScale(dash.powerTextScale);
    dash.powerText.setPosition(dash.dashPos.x+135.0f, dash.dashPos.y+8.0f);

    dash.bodyAngleText.setFont(g.assetMan.getFont());
    dash.bodyAngleText.setScale(dash.powerTextScale);
    dash.bodyAngleText.setPosition(dash.dashPos.x+135.0f, dash.dashPos.y+33.0f);

    dash.turretAngleText.setFont(g.assetMan.getFont());
    dash.turretAngleText.setScale(dash.turretAngleTextScale);
    dash.turretAngleText.setPosition(dash.dashPos.x+200.0f, dash.dashPos.y+100.0f);

    dash.velText.setFont(g.assetMan.getFont());
    dash.velText.setScale(dash.velTextScale);
    dash.velText.setPosition(dash.dashPos.x+200.0f, dash.dashPos.y+100.0f);

    dash.posText.setFont(g.assetMan.getFont());
    dash.posText.setScale(dash.posTextScale);
    dash.posText.setPosition(dash.dashPos.x+200.0f, dash.dashPos.y+100.0f);
    
    dashView.reset(sf::FloatRect(0.0f,0.0f, (float)g.scrWidth, (float)g.scrHeight));   
    window.setView(dashView);//set view to dash mode..
    window.draw(dash.healthText);
    window.draw(dash.speedText);
    window.draw(dash.powerText);
    window.draw(dash.bodyAngleText);

    /*if (g.teamMan.teams[g.myTeam].base1.health <= 0 && g.teamMan.teams[(g.myTeam==1 ? 2 : 1)].base1.health > 0)
    {
         window.draw(dash.turretAngleText);
    }
    else if (g.teamMan.teams[(g.myTeam==1 ? 2 : 1)].base1.health <= 0 && g.teamMan.teams[g.myTeam].base1.health > 0)
    {
        window.draw(dash.velText);
    }else if (g.teamMan.teams[g.myTeam].base1.health <= 0 && g.teamMan.teams[(g.myTeam==1 ? 2 : 1)].base1.health <= 0)
    {
        window.draw(dash.posText);
    }*/
    
    //
    //dashTexture.display();
    //dashSprite.setTexture(dashTexture.getTexture());
    //
    //dashView.reset(sf::FloatRect(0.0f,0.0f, (float)g.scrWidth, (float)g.scrHeight));   
    //window.setView(dashView);//set view to dash mode..
    //window.draw(dashSprite);
    //window.setView(arenaView);//Set view back to normal mode.

    
    /*sf::View miniView = arenaView;
    miniView.setViewport(sf::FloatRect(0.75,0.75,0.25,0.25));
    miniView.zoom(15);
    window.setView(miniView);
    window.draw(entityVertices, &g.assetMan.getTexture(0));
    window.setView(arenaView);*/
    
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

