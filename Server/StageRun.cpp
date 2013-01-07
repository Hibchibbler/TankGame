#include "StageRun.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "GameServer.h"
using namespace tg;

#define PROJECTILE_DISTANCE 10

#define RADTODEG(rad) rad*57.2963750f;
#define DEGTORAD(deg) deg*0.17300866f;

StageRun::StageRun(Game & g)
    : GameStage(g)
{
    loopTime = sf::Time();
    stateUpdated = false;
}

sf::Uint32 StageRun::doRemoteEvent(Game & g,
                                   CommEvent & cevent,
                                   sf::Uint32 connId,
                                   sf::Uint32 msgId)
{
    Player &player = g.teamMan.getPlayer(connId);

    switch (msgId){
       
        case MsgId::WhoIs:
            std::cout << "Got OOB WhoIs" << std::endl;
            //If they want to know, then we'll tell em.
            player.state = PlayerState::SendingWhoIsAck;
            break;
        case MsgId::Id:{
            //They cannot reseat themselves in this stage.
            std::cout << "Got OOB Id" << std::endl;
            player.state = PlayerState::SendingIdNack;
            break;
        }case MsgId::StateOfPlayer:{
            //std::cout << "Got Action" << std::endl;
            //These are from players when they change the state of the game via an input mechanism
            //After crunching this event, send a Result packet to each player.

            //std::cout << << std::endl;
            //sf::Uint32 playerAction;
            
            sf::Uint32 attacking = 0;
           //float temp1,temp2,temp3,temp4;

            cevent.packet >> player.tank.throttle;
            cevent.packet >> player.tank.bodyAngle;
            cevent.packet >> player.tank.turretAngle;
            //cevent.packet >> temp1;//player.tank.position.x;
            //cevent.packet >> temp2;//player.tank.position.y;
            //cevent.packet >> temp3;//player.tank.velocity.x;
            //cevent.packet >> temp4;//player.tank.velocity.y;
            cevent.packet >> attacking;

            if (attacking){
                //std::cout << "#";
                player.state = PlayerState::EmitProjectile;
            }
            //update player 1 velocity based on updated throttle and body angle.
            /*player.tank.position.x =  player.tank.position.x + player.tank.throttle * (float)cos(player.tank.bodyAngle / (180/3.14156));
            player.tank.position.y =  player.tank.position.y + player.tank.throttle * (float)sin(player.tank.bodyAngle / (180/3.14156));*/
            player.tank.velocity.x = player.tank.throttle * (float)cos(player.tank.bodyAngle * (0.0174531f));
            player.tank.velocity.y = player.tank.throttle * (float)sin(player.tank.bodyAngle * (0.0174531f));
            //std::cout << player.tank.position.x << ", " << player.tank.position.y << std::endl;
            break;
        }
    }
    return 0;
}

bool isContained(sf::Vector2f pt, float left, float top, float width, float height)
{
    if (pt.x <left+width && pt.x > left &&
        pt.y > top       && pt.y < top+height)
    {
        return true;
    }
    return false;
   // return fr.contains(pt);
}

bool isTankCollision(sf::Vector2f projPos, Game & g, sf::Int32 damage, sf::Uint32 team=-1)
{
    for (int y = 1; y < 3 ; y++)
    {
        //skip doing collision if a safe team is specified
        if (y != -1 && team == y)
            continue;
        tg::Team::PlayerIterator tp = g.teamMan.getTeam(y).begin();
        for (;tp != g.teamMan.getTeam(y).end();tp++){
            //tp->tank.position
            if (tp->hasHost)
            {
                if (projPos.x > tp->tank.position.x &&
                    projPos.x < tp->tank.position.x+60.0f &&
                    projPos.y > tp->tank.position.y && 
                    projPos.y < tp->tank.position.y+30.0f )
                {
                    tp->tank.health-=damage;
                    if (tp->tank.health<=0)
                        tp->tank.health=0;
                    
                    return true;
                }
            }
        }
    }

    return false;

    ////TODO: assumes size of tank.
}

int isCreepCollision(sf::Vector2f projPos, Game & g, sf::Uint32 damage, sf::Uint32 team=-1)
{
    for (int y = 1; y < 3 ; y++)
    {
        //skip doing collision if a safe team is specified
        if (y != -1 && team == y)
            continue;
        
        for (auto c = g.teamMan.teams[y].creep.begin();c != g.teamMan.teams[y].creep.end();)
        {
            //tp->tank.position
         
                if (projPos.x > c->position.x &&
                    projPos.x < c->position.x+60.0f &&
                    projPos.y > c->position.y && 
                    projPos.y < c->position.y+30.0f )
                {
                    c->health-=damage;
                    if (c->health <= 0)
                    {
                        c = g.teamMan.teams[y].creep.erase(c);
                        return 2;
                    }
                    return 1;
                }else
                    c++;
            
        }
    }

    return 0;

    ////TODO: assumes size of tank.
}

bool isGeneratorCollision(sf::Vector2f projPos, Game & g, sf::Uint32 damage, sf::Uint32 team=-1)
{
    for (int y = 1; y < 3 ; y++)
    {
        //skip doing collision if a safe team is specified
        if (y != -1 && team == y)
            continue;
        sf::Vector2f genPos = g.arenaMan.getGeneratorPosition(y);

        if (projPos.x > genPos.x &&
            projPos.x < genPos.x+125.0f &&
            projPos.y > genPos.y && 
            projPos.y < genPos.y+125.0f )
        {
            
            g.teamMan.teams[y].gen1.health-=damage;
            if (g.teamMan.teams[y].gen1.health<=0)
                g.teamMan.teams[y].gen1.health=0;
            return true;
        }
    }
    return false;
}
bool isBaseCollision(sf::Vector2f projPos, Game & g, sf::Uint32 damage, sf::Uint32 team=-1)
{
    for (int y = 1; y < 3 ; y++)
    {
        //skip doing collision if a safe team is specified
        if (y != -1 && team == y)
            continue;
        sf::Vector2f genPos = g.arenaMan.getStartPosition(y);

        if (projPos.x > genPos.x &&
            projPos.x < genPos.x+125.0f &&
            projPos.y > genPos.y && 
            projPos.y < genPos.y+125.0f )
        {
            g.teamMan.teams[y].base1.health-=damage;
            if (g.teamMan.teams[y].base1.health<=0)
                g.teamMan.teams[y].base1.health=0;
            return true;
        }
    }
    return false;
}

#define CREEP_SPEED 17
#define CREEP_SPAWN_MS 1400
#define UPDATE_STATE_MS 30
#define SEND_STATE_MS 100//110
#define PIXELS_PER_SECOND 10
#define CREEP_LIFE_S 75.0f
#define BASE_LASER_PROXIMAL 1100.0f
#define BASE_LASER_LIFE_S 0.80f
#define BASE_LASER_SPEED 120.0f
#define HEAL_LASER_SPAWN_S 0.05f
#define DEATH_LASER_SPAWN_S 0.04f

//STATE_OF_UNION_MS
sf::Uint32 StageRun::doLoop(Game & g)
{
    
    //std::cout << currentTime.asSeconds() << std::endl;
    //Players, teams, and global Update
    if (updateStateTimer.getElapsedTime().asMilliseconds() > UPDATE_STATE_MS)
    {
        stateUpdated = true;
        previousTime = currentTime;
        currentTime = velocityClock.restart();
        deltaTime = currentTime - previousTime;
        loopTime += deltaTime;
        for (int y = 1; y < 3 ; y++)
        {
            //Players update
            tg::Team::PlayerIterator pi = g.teamMan.getTeam(y).begin();
            for (;pi != g.teamMan.getTeam(y).end();pi++){
        
                if (pi->hasHost == false)
                    continue;
    
                switch (pi->state){
                    case PlayerState::SendingWhoIsAck:
                        Messages::sendWhoIsAck(g.server, g.teamMan, pi->connectionId);
                        pi->state = PlayerState::Running; 
                        break;
                    case PlayerState::SendingIdNack:
                        Messages::sendIdNack(g.server, g.teamMan, pi->connectionId);
                        pi->state = PlayerState::Running;
                        break;
                    case PlayerState::Ready:
                        Messages::sendStart(g.server, g.teamMan, pi->connectionId);
                        pi->state = PlayerState::Running;
                        break;
                    case PlayerState::EmitProjectile:
                    {
                        pi->prjctls.push_back(Projectile());
                        //TODO: THINK: do you want projectile to inherit the tanks' velocity?
                    
                        pi->prjctls.back().velocity.x = 75.0f*((float)cos(pi->tank.turretAngle*(0.0174531f)));
                        pi->prjctls.back().velocity.y = 75.0f*((float)sin(pi->tank.turretAngle*(0.0174531f)));

                        float vx,vy;
                        vx =  pi->prjctls.back().velocity.x;
                        vy =  pi->prjctls.back().velocity.y;
                        //float mag = sqrt(vx*vx+vy*vy);
                        //g.teamMan.teams[y].base1.prjctls.back().velocity.x = -BASE_LASER_SPEED*mindx/mag; 
                        //g.teamMan.teams[y].base1.prjctls.back().velocity.y = -BASE_LASER_SPEED*mindy/mag;
                        pi->prjctls.back().position.x = pi->tank.position.x + (vx/75.0f)*2.5f;
                        pi->prjctls.back().position.y = pi->tank.position.y + (vy/75.0f)*2.5f;
                    
                        //std::cout << pi->prjctls.back().position.x << ", " << pi->prjctls.back().position.y << std::endl;
                        pi->prjctls.back().creationTime = accumulatingClock.getElapsedTime().asSeconds();
                        pi->prjctls.back().angle = pi->tank.turretAngle;
                        pi->prjctls.back().damage = 1+pi->tank.power/10.0f;
                        pi->state = PlayerState::Running;
                        //break;
                    }
                    case PlayerState::Running:
                         //Remove projectiles who have run out of power.
                        for (auto i = pi->prjctls.begin();i != pi->prjctls.end();)
                        {
                            float accTime = accumulatingClock.getElapsedTime().asSeconds();
                            if ( ((i->creationTime + 0.5f) + (pi->tank.power/20.0f))  < accTime )// (PROJECTILE_DISTANCE*500)+(pi->tank.power*25))
                            {
                                i = pi->prjctls.erase(i);
                            }else
                            {
                               /* if ((i->position.x + i->velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND < (15*125) &&
                                    i->position.x + i->velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND > (14*125) &&
                                    i->position.y + i->velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND > (12*125) &&
                                    i->position.y + i->velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND < (19*125)) ||
                                    (i->position.x + i->velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND < (18*125) &&
                                    i->position.x + i->velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND > (11*125) &&
                                    i->position.y + i->velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND > (15*125) &&
                                    i->position.y + i->velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND < (16*125)))
                                {
                                    i->velocity.x = 0;
                                    i->velocity.y = 0;
                                }*/

                                i->position.x = i->position.x + i->velocity.x * loopTime.asSeconds()*PIXELS_PER_SECOND;
                                i->position.y = i->position.y + i->velocity.y * loopTime.asSeconds()*PIXELS_PER_SECOND;
                                i->totalDistance+=1;

                                ////Remove projectile that has hit a tank or creep
                                bool yes1 = isTankCollision(i->position,g,i->damage,y);
                                int yes2 = isCreepCollision(i->position,g,i->damage,y);
                                bool yes3 = isGeneratorCollision(i->position,g, i->damage,y);
                                bool yes4 = isBaseCollision(i->position,g, i->damage,y);
                            
                                if (yes1!=0 || yes2 || yes3 || yes4)
                                {
                                    i = pi->prjctls.erase(i);
                                    if (yes2==2)
                                        pi->tank.power++;
                                    
                                }
                                else
                                    i++;
                            }
                        }

                        //TODO: HACK: hardcoded arena boundaries based on debug map and a 125px wide floor tile.
                        if (pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND < (125) ||
                            pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND > (59*125) )
                        {
                            pi->tank.velocity.x = 0;
                        }

                        if (pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND < (125) ||
                            pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND > (59*125) )
                        {
                            pi->tank.velocity.y = 0;
                        }

                       /* if ((pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND < (15*125) &&
                            pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND > (14*125) &&
                            pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND > (12*125) &&
                            pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND < (19*125)) ||
                            (pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND < (18*125) &&
                            pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND > (11*125) &&
                            pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND > (15*125) &&
                            pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND < (16*125)))
                        {
                             pi->tank.velocity.x = 0;
                             pi->tank.velocity.y = 0;
                        }*/

                   

                        pi->tank.position.x = pi->tank.position.x + pi->tank.velocity.x *  loopTime.asSeconds()*PIXELS_PER_SECOND;
                        pi->tank.position.y = pi->tank.position.y + pi->tank.velocity.y *  loopTime.asSeconds()*PIXELS_PER_SECOND;
            
                        break;
                }
            }
            //Teams Update
            float accumulatedClock = accumulatingClock.getElapsedTime().asSeconds();

            //Generator LASER
            // Determine the closest opponent tank for damage
            // Determine the closest friendly tank for heal
            
            float minDistOther = 400000000.0f;
            float mindxOther, mindyOther;
            int minIndexOther=-1;

            float minDistUs = 400000000.0f;
            float mindxUs, mindyUs;
            int minIndexUs=-1;

            int otherTeam = (y==1?2:1);

            
            sf::Vector2f genPos = g.arenaMan.getGeneratorPosition(y);
            sf::Vector2f basePos = g.arenaMan.getStartPosition(y);
            
            //Do Generator Death Ray
            if (deathLaserClock.getElapsedTime().asSeconds() > DEATH_LASER_SPAWN_S)
            {
                for (auto p = 0;p < g.teamMan.teams[otherTeam].players.size();p++){
                    Player & player = g.teamMan.teams[otherTeam].players[p];
                    if (player.hasHost == false)
                        continue;

                
                    //Calc distance between generator and player
                    float dx,dy;
                    dx = genPos.x - player.tank.position.x;
                    dy = genPos.y - player.tank.position.y;
                    float dist = sqrt(dx*dx+dy*dy);
                
                    if (dist < minDistOther){
                        minDistOther = dist;
                        minIndexOther = p;
                        mindxOther = dx;
                        mindyOther = dy;
                    }

                }

                // If closest opponent is closer than BASE_LASER_PROXIMAL
                // fire at it!
                if (minIndexOther != -1 && g.teamMan.teams[y].gen1.health > 0 && minDistOther <= BASE_LASER_PROXIMAL)
                {
                    g.teamMan.teams[y].gen1.prjctls.push_back(Projectile());
                    
                    
                    float mag = sqrt(mindxOther*mindxOther+mindyOther*mindyOther);
                    g.teamMan.teams[y].gen1.prjctls.back().velocity.x = -BASE_LASER_SPEED*mindxOther/mag; 
                    g.teamMan.teams[y].gen1.prjctls.back().velocity.y = -BASE_LASER_SPEED*mindyOther/mag;

                    g.teamMan.teams[y].gen1.prjctls.back().position.x = genPos.x;
                    g.teamMan.teams[y].gen1.prjctls.back().position.y = genPos.y;

                    g.teamMan.teams[y].gen1.prjctls.back().angle = atan2(mindyOther,mindxOther)*(180.0f/3.141567f)-90.0f;

                    g.teamMan.teams[y].gen1.prjctls.back().creationTime = accumulatingClock.getElapsedTime().asSeconds();
                    g.teamMan.teams[y].gen1.prjctls.back().damage = 1;
                }
            }
            //For each generator laser segment, keep it moving. also, remove expired segments.
            for (auto b = g.teamMan.teams[y].gen1.prjctls.begin();b != g.teamMan.teams[y].gen1.prjctls.end();)
            {
                b->position.x = b->position.x + b->velocity.x*loopTime.asSeconds()*PIXELS_PER_SECOND;
                b->position.y = b->position.y + b->velocity.y*loopTime.asSeconds()*PIXELS_PER_SECOND;

                bool yes = isTankCollision(b->position,g,b->damage,y);
                if (yes || b->creationTime + BASE_LASER_LIFE_S < accumulatedClock)
                {
                    b = g.teamMan.teams[y].gen1.prjctls.erase(b);
                }else
                {
                    b++;
                }
            }

            //Do Base Heal Ray
            if (healLaserClock.getElapsedTime().asSeconds() > HEAL_LASER_SPAWN_S)
            {
                for (auto p = 0;p < g.teamMan.teams[y].players.size();p++){
                    Player & player = g.teamMan.teams[y].players[p];
                    if (player.hasHost == false)
                        continue;

                
                    //Calc distance between generator and player
                    float dx,dy;
                    dx = basePos.x - player.tank.position.x;
                    dy = basePos.y - player.tank.position.y;
                    float dist = sqrt(dx*dx+dy*dy);
                
                    if (dist < minDistUs){
                        minDistUs = dist;
                        minIndexUs = p;
                        mindxUs = dx;
                        mindyUs = dy;
                    }

                }

                // If closest ally is closer than BASE_LASER_PROXIMAL
                // fire at it!
                if (minIndexUs != -1 && g.teamMan.teams[y].base1.health > 0 && g.teamMan.teams[y].players[minIndexUs].tank.health < 100 && minDistUs <= BASE_LASER_PROXIMAL)
                {
                    g.teamMan.teams[y].base1.prjctls.push_back(Projectile());
                    
                    
                    float mag = sqrt(mindxUs*mindxUs+mindyUs*mindyUs);
                    g.teamMan.teams[y].base1.prjctls.back().velocity.x = -BASE_LASER_SPEED*mindxUs/mag; 
                    g.teamMan.teams[y].base1.prjctls.back().velocity.y = -BASE_LASER_SPEED*mindyUs/mag;

                    g.teamMan.teams[y].base1.prjctls.back().position.x = basePos.x;
                    g.teamMan.teams[y].base1.prjctls.back().position.y = basePos.y;

                    g.teamMan.teams[y].base1.prjctls.back().angle = atan2(mindyUs,mindxUs)*(180.0f/3.141567f)-90.0f;

                    g.teamMan.teams[y].base1.prjctls.back().creationTime = accumulatingClock.getElapsedTime().asSeconds();
                    g.teamMan.teams[y].base1.prjctls.back().damage = 1;
                }
            }
            //For each base laser segment, keep it moving. also, remove expired segments.
            for (auto b = g.teamMan.teams[y].base1.prjctls.begin();b != g.teamMan.teams[y].base1.prjctls.end();)
            {
                b->position.x = b->position.x + b->velocity.x*loopTime.asSeconds()*PIXELS_PER_SECOND;
                b->position.y = b->position.y + b->velocity.y*loopTime.asSeconds()*PIXELS_PER_SECOND;

                bool yes = isTankCollision(b->position,g,-b->damage,-1);
                if (yes || b->creationTime + BASE_LASER_LIFE_S < accumulatedClock)
                {
                    b = g.teamMan.teams[y].base1.prjctls.erase(b);
                }else
                {
                    b++;
                }
            }

            //Spawn new creep
            if (minionAddClock.getElapsedTime().asMilliseconds() > CREEP_SPAWN_MS)
            {
                if (g.teamMan.teams[y].base1.health > 0)
                {
                    int r=0;
                    Creep newCreep;
                    newCreep.position = g.arenaMan.getStartPosition(y);
                    newCreep.health = 8;
                    newCreep.creationTime = accumulatedClock;

                    //randomly spawn to the right, or left. stay away from center.
                    //Team 2 is 180 rotated, but the same.
                    if (y==1)
                    {
                        int q = rand()%2;
                        if (q == 0)
                        {
                            r = (rand()%30)+60+15;
                        }else if (q==1)
                        {
                            r = (rand()%30)-15;
                        }
                    }else if (y ==2)
                    {
                       int q = rand()%2;
                        if (q == 0)
                        {
                            r = ((rand()%30)+60+15)+180;
                        }else if (q==1)
                        {
                            r = (rand()%30)+180-15;
                        }
                    }
                    newCreep.velocity.x = CREEP_SPEED*cos(r*0.0174531f);
                    newCreep.velocity.y = CREEP_SPEED*sin(r*0.0174531f);

                    g.teamMan.teams[y].creep.push_back(newCreep);
                }
            }
        
            //For each creep
            for (auto c = g.teamMan.teams[y].creep.begin();c != g.teamMan.teams[y].creep.end();)
            {
                //Move this creep
                c->position.x = c->position.x + c->velocity.x * loopTime.asSeconds()*PIXELS_PER_SECOND;
                c->position.y = c->position.y + c->velocity.y * loopTime.asSeconds()*PIXELS_PER_SECOND;

                int teami = (y==1?2:1);
                for (auto tanki = 0;tanki < g.teamMan.teams[teami].players.size();tanki++)
                {
                    if (g.teamMan.teams[teami].players[tanki].hasHost)
                    {
                        //sf::FloatRect tankR(g.teamMan.teams[teami].players[tanki].tank.position,
                        float dx = c->position.x - g.teamMan.teams[teami].players[tanki].tank.position.x;
                        float dy = c->position.y - g.teamMan.teams[teami].players[tanki].tank.position.y;
                    
                    
                        if (sqrt(dx*dx+dy*dy) < 700)
                        {
                            float ang =  atan2(dy,dx) / 0.0174531f;
                        
                            c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                            c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                        
                        }
                    }
                    
                }

                ////Remove creep that is hit a tank or another creep
                bool yes = isTankCollision(c->position,g, 5, y);
                int yes2 = isCreepCollision(c->position,g,5,y);
                if (yes || (yes2 && rand()%2==1) || c->creationTime + CREEP_LIFE_S < accumulatedClock )
                {
                    c = g.teamMan.teams[y].creep.erase(c);
                }else
                    c++;

            }
        
        }
        //Global update

        if (minionAddClock.getElapsedTime().asMilliseconds() > CREEP_SPAWN_MS)
            minionAddClock.restart();

        if (healLaserClock.getElapsedTime().asSeconds() > HEAL_LASER_SPAWN_S)
            healLaserClock.restart();

        if (deathLaserClock.getElapsedTime().asSeconds() > DEATH_LASER_SPAWN_S)
            deathLaserClock.restart();
        
        updateStateTimer.restart();
    }else{//not time to update state.
        sf::sleep(sf::milliseconds(0));
    }

    if (sendStateTimer.getElapsedTime().asMilliseconds() > SEND_STATE_MS && stateUpdated)
    {
        Messages::sendStateOfUnion(g.server, g.teamMan);
        sendStateTimer.restart();
    }
    return getSummary(0).a;
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, Game & g)
{

    return 0;
}



