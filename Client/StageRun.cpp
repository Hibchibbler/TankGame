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
}

sf::Uint32 StageRun::doInit(Game & g)
{
    //Load HUD
    dash.load();

    //Prepare obstruction list(once)
    obstructionList = prepareObstructionList(g.arenaMan);
    
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
        {
            zoom += 1.f;
            
            if (zoom > 8.0f){//prevent from getting to zoomed out
               zoom = 8.0f;
            }
        }else{
            zoom -= 1.f;
            
            if (zoom < 1.0f){//prevent from getting too zoomed in
                zoom=1.0f;
            }

        }
        std::cout << "Zoom: " << zoom << std::endl;
    }else if (event.type == sf::Event::MouseEntered){
        hasFocus = true;
    }else if (event.type == sf::Event::MouseLeft){
        hasFocus = false;
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
    if (inputClock.getElapsedTime().asSeconds() > 0.100f)
    {
        if (!hasFocus)
            return 0;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            showShadow=!showShadow;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            showRunMenu=!showRunMenu;
            if (showRunMenu){
                mywindow->Show(true);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        {
            visionRange+=250;
        }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            visionRange-=250;
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

        times.push_back(frameTime2.asSeconds());
        if (times.size() > 20)
            times.erase(times.begin());
        float at=0.0f;
        for (auto k = times.begin();k != times.end();k++){
            at += *k;
        }
        at /= 20.0f;
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

sf::Uint32 addStraightQuad(sf::VertexArray & v, sf::FloatRect c, sf::IntRect t)
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


sf::Uint32 addRotQuad(sf::VertexArray & v, sf::FloatRect p, sf::IntRect t, float angle)
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
    return 0;
}

#define VISION_RANGE 1000

sf::Uint32 StageRun::drawAll(sf::RenderWindow & window, Game & g)
{   

    previousTime2 = currentTime2;
    currentTime2 = clock2.restart();
    frameTime2 += currentTime2 - previousTime2;
    //std::cout << 1.0f/frameTime2.asSeconds() << std::endl;
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

    pos = thisPlayer.tank.position;

    sf::FloatRect fr = sf::FloatRect(::floor(pos.x-(g.scrWidth/2.0f)),::floor(pos.y-((g.scrHeight)/2.0f)),g.scrWidth,g.scrHeight);
    arenaView.reset(fr);
    arenaView.zoom(zoom);
    window.setView(arenaView);


    if (updateFloorClock.getElapsedTime().asMilliseconds() > 400)
    {
        sf::Sprite fog = g.assetMan.getSprite(ImageType::FogOfWar);
        floorVertices.clear();
        for (int i = 0;i < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum();i++){
            Tile &tile = g.arenaMan.getTile(i);
            sf::Sprite s = g.assetMan.getSprite(tile.getId()+4);
            float dx,dy;
            dx = tile.getPosition().x+64 - pos.x;
            dy = tile.getPosition().y+64 - pos.y;

            /*float dxb, dyb;
            dxb = tile.getPosition().x - g.arenaMan.getStartPosition(g.myTeam).x;
            dyb = tile.getPosition().y - g.arenaMan.getStartPosition(g.myTeam).y;*/

            if (abs(dx) < (g.scrWidth-257)*zoom &&
                abs(dy) < (g.scrHeight-257)*zoom)
            {
                addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), s.getTextureRect());
                if (sqrt(dx*dx+dy*dy) > visionRange)// || sqrt(dxb*dxb+dyb*dyb) < visionRange*1.75f)
                {
                    addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), s.getTextureRect());
                    addStraightQuad(floorVertices, sf::FloatRect(tile.getPosition().x,tile.getPosition().y, 128,128), fog.getTextureRect());
                }
            }
        }
        updateFloorClock.restart();
    }
    window.draw(floorVertices, &g.assetMan.getTexture(0));


    //Draw the tanks    
    //  Draw My Locally Predicted Tank
    entityVertices.clear();
    //Remote Players Tanks
    for (int y=1;y < 3;y++)
    {
        sf::Sprite b = g.assetMan.getSprite((y==1 ? ImageType::TankBlue : ImageType::TankRed));
        sf::Sprite t = g.assetMan.getSprite((y==1 ? ImageType::TurretBlue : ImageType::TurretRed));

        for (int h = 0;h < g.teamMan.teams[y].players.size();h++)
        {
            if (g.teamMan.teams[y].players[h].hasHost)
            {
                
                if (g.myTeam == y && g.mySlot == h)
                {
                    //Draw You
                    addRotQuad(entityVertices,
                               sf::FloatRect(thisPlayer.tank.position.x, thisPlayer.tank.position.y, 73,116),
                               b.getTextureRect(),
                               thisPlayer.tank.bodyAngle-STD_ROTATE_OFFSET);

                    addRotQuad(entityVertices,
                               sf::FloatRect(thisPlayer.tank.position.x,thisPlayer.tank.position.y, 47,176),
                               t.getTextureRect(),
                               thisPlayer.tank.turretAngle-STD_ROTATE_OFFSET);
                    
                }else
                {
                    //Draw Them
                    float dx,dy;
                    dx = g.teamMan.teams[y].players[h].tank.position.x - pos.x;
                    dy = g.teamMan.teams[y].players[h].tank.position.y - pos.y;

                    if (sqrt(dx*dx+dy*dy) < visionRange || g.myTeam == y)
                    {
                        addRotQuad(entityVertices,
                                   sf::FloatRect(g.teamMan.teams[y].players[h].tank.position.x, g.teamMan.teams[y].players[h].tank.position.y, 73,116),
                                   b.getTextureRect(),
                                   g.teamMan.teams[y].players[h].tank.bodyAngle-STD_ROTATE_OFFSET);

                        addRotQuad(entityVertices,
                                   sf::FloatRect(g.teamMan.teams[y].players[h].tank.position.x, g.teamMan.teams[y].players[h].tank.position.y, 47,176),
                                   t.getTextureRect(),
                                   g.teamMan.teams[y].players[h].tank.turretAngle-STD_ROTATE_OFFSET);
                    }
                }
            }
        }
    }

    sf::Sprite p = g.assetMan.getSprite(ImageType::Projectile1);
    for (int y=1;y < 3;y++)
    {
        for (int h = 0;h < g.teamMan.teams[y].players.size();h++)
        {
            if (g.teamMan.teams[y].players[h].hasHost)
            {
                if (g.myTeam == y && g.mySlot == h)
                {
                    //Draw Projectiles
                    for (int k = 0;k < thisPlayer.prjctls.size();k++)
                    {

                        addRotQuad(entityVertices,
                                   sf::FloatRect(thisPlayer.prjctls[k].position.x, thisPlayer.prjctls[k].position.y, 32,32),
                                   p.getTextureRect(),
                                   0.0f);
                    }
                }else{
                    for (int k = 0;k < g.teamMan.teams[y].players[h].prjctls.size();k++)
                    {
                        float dx,dy;
                        dx = g.teamMan.teams[y].players[h].prjctls[k].position.x - pos.x;
                        dy = g.teamMan.teams[y].players[h].prjctls[k].position.y - pos.y;

                        if (sqrt(dx*dx+dy*dy) < visionRange)
                        {
                            addRotQuad(entityVertices,
                                       sf::FloatRect(g.teamMan.teams[y].players[h].prjctls[k].position.x, g.teamMan.teams[y].players[h].prjctls[k].position.y, 32,32),
                                       p.getTextureRect(),
                                       0.0f);
                        }
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
                float dx,dy;
                dx = g.teamMan.teams[y].creep[lk].position.x - pos.x;
                dy = g.teamMan.teams[y].creep[lk].position.y - pos.y;

           /*     float dxb, dyb;
                dxb = g.teamMan.teams[y].creep[lk].position.x - g.arenaMan.getStartPosition(g.myTeam).x;
                dyb = g.teamMan.teams[y].creep[lk].position.y - g.arenaMan.getStartPosition(g.myTeam).y;
                */

                if (sqrt(dx*dx+dy*dy) < visionRange)// || sqrt(dxb*dxb+dyb*dyb) < visionRange*1.75f)
                {
                    addRotQuad(entityVertices,
                               sf::FloatRect(g.teamMan.teams[y].creep[lk].position.x, g.teamMan.teams[y].creep[lk].position.y, 64,64),
                               creep.getTextureRect(),
                               g.teamMan.teams[y].creep[lk].angle);
                }
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
                    float dx,dy;
                    dx = g.teamMan.teams[y].gen[gi].prjctls[k].position.x - pos.x;
                    dy = g.teamMan.teams[y].gen[gi].prjctls[k].position.y - pos.y;

                    if (sqrt(dx*dx+dy*dy) < visionRange)
                    {
                        addRotQuad(entityVertices,
                                   sf::FloatRect(g.teamMan.teams[y].gen[gi].prjctls[k].position.x, g.teamMan.teams[y].gen[gi].prjctls[k].position.y, 64,64),
                                   prjctl.getTextureRect(),
                                   g.teamMan.teams[y].gen[gi].prjctls[k].angle);
                    }
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
                float dx,dy;
                dx = g.teamMan.teams[y].base1.prjctls[k].position.x - pos.x;
                dy = g.teamMan.teams[y].base1.prjctls[k].position.y - pos.y;

                if (sqrt(dx*dx+dy*dy) < visionRange)
                {
                    addRotQuad(entityVertices,
                               sf::FloatRect(g.teamMan.teams[y].base1.prjctls[k].position.x, g.teamMan.teams[y].base1.prjctls[k].position.y, 64,64),
                               prjctl.getTextureRect(),
                               g.teamMan.teams[y].base1.prjctls[k].angle);
                }
            }
        }
    }

    
    window.draw(entityVertices, &g.assetMan.getTexture(0));
    

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
           
            float dx,dy;
            dx = e->position.x-128 - pos.x;
            dy = e->position.y-128 - pos.y;

            if (sqrt(dx*dx+dy*dy) < visionRange)
            {

                addStraightQuad(explosionSmallVertices,
                            sf::FloatRect(e->position.x-128, e->position.y-128, 256,256),
                            sf::IntRect(xi,yi,64,64));
            }
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
    window.draw(explosionSmallVertices, &g.assetMan.getTexture(1));

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
            
            float dx,dy;
            dx = e->position.x-128 - pos.x;
            dy = e->position.y-128 - pos.y;

            if (sqrt(dx*dx+dy*dy) < visionRange)
            {
                addStraightQuad(explosionBigVertices,
                            sf::FloatRect(e->position.x-128, e->position.y-128, 256,256),
                            sf::IntRect(xi,yi,64,64));
            }
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
    window.draw(explosionBigVertices, &g.assetMan.getTexture(2));

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
    
    dashView.reset(sf::FloatRect(0.0f,0.0f, (float)g.scrWidth, (float)g.scrHeight));   
    window.setView(dashView);//set view to dash mode..
    window.draw(dash.healthText);
    window.draw(dash.speedText);
    window.draw(dash.powerText);
    window.draw(dash.bodyAngleText);
    //    
    //dashTexture.display();
    //dashSprite.setTexture(dashTexture.getTexture());
    //
    //dashView.reset(sf::FloatRect(0.0f,0.0f, (float)g.scrWidth, (float)g.scrHeight));   
    //window.setView(dashView);//set view to dash mode..
    //window.draw(dashSprite);
    //window.setView(arenaView);//Set view back to normal mode.


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

