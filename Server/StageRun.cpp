#include "StageRun.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "Common\Physics.h"

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

sf::Uint32 StageRun::doInit(Game &g)
{
    //Prepare obstruction list(once)
    obstructionList = prepareObstructionList(g.arenaMan);
    return 0;
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
            //These are from players when they change the state of their tank
            cevent.packet >> player.tank.turretAngle;
            cevent.packet >> player.tank.accelerating;
            cevent.packet >> player.tank.turningBody;
            cevent.packet >> player.tank.attacking;

            
            //std::cout << player.tank.position.x << ", " << player.tank.position.y << std::endl;
            break;
        }
    }
    return 0;
}


#define CREEP_SPEED 17
#define CREEP_SPAWN_MS 1000
#define UPDATE_STATE_MS 25
#define SEND_STATE_MS 100//110
#define PIXELS_PER_SECOND 10
#define CREEP_LIFE_S 130.0f
#define HEAL_LASER_PROXIMAL 450.0f
#define DEATH_LASER_PROXIMAL 1100.0f
#define HEAL_LASER_LIFE_S 0.65f
#define DEATH_LASER_LIFE_S 0.80f
#define HEAL_LASER_SPEED 55.0f
#define DEATH_LASER_SPEED 120.0f
#define HEAL_LASER_SPAWN_S 0.07f
#define DEATH_LASER_SPAWN_S 0.005f
#define CREEP_TANK_PROXIMAL 800
#define CREEP_CREEP_PROXIMAL 700
#define CREEP_GENERATOR_PROXIMAL 500
#define CREEP_BASE_PROXIMAL 500
#define CREEP_WAYPOINT_PROXIMAL 100

int getNewCreepAngle1(sf::Uint32 team)
{
    int r=0;
    if (team==1)
    {
        int q = rand()%2;
        if (q == 0)
        {
            r = (rand()%30)+60+15;
        }else if (q==1)
        {
            r = (rand()%30)-15;
        }
    }else// if (team ==2)
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
    return r;
}

int getNewCreepAngle2(sf::Uint32 team)
{
    int r=30;
    if (team==1)
    {
        int q = rand()%3;
        if (q == 0)
        {
            r = 14+(rand()%10);
        }else if (q==1)
        {
            r = -14-(rand()%10);
        }else{
            r = -5+(rand()%10);
        }
    }else// if (team ==2)
    {
       int q = rand()%3;
        if (q == 0)
        {
            r = 14+(rand()%10);
        }else if (q==1)
        {
            r = -14-(rand()%10);
        }else{
            r = -5+(rand()%10);
        }
        r+=180;//team 1 is opposite..
    }
    return r;
}


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

        //std::cout << loopTime.asSeconds() << std::endl;
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
                    case PlayerState::Running:
                        fireProjectile(*pi, accumulatingClock);
                        updateVelocity(*pi, obstructionList, loopTime);
                        updateProjectilCollisions(g,*pi, y,obstructionList, false,loopTime, accumulatingClock);
                        break;
                }
            }
            //Teams Update
            bool client = false;///the following code needs to be put into MeAndMyTank
                                //this hack exists due to that fact.

            float accumulatedClock = accumulatingClock.getElapsedTime().asSeconds();

            //Generator LASER
            // Determine the closest opponent tank for damage
            // Determine the closest friendly tank for heal

            int otherTeam = (y==1?2:1);

            //sf::Vector2f genPos = g.arenaMan.getGeneratorPosition(y);
            sf::Vector2f basePos = g.arenaMan.getStartPosition(y);
            
            //Do Generator Death Ray
            if (deathLaserClock.getElapsedTime().asSeconds() > DEATH_LASER_SPAWN_S)
            {
                for (int gi=0;gi < g.arenaMan.getGeneratorCount(y);gi++)
                {
                    float minDistOther = 400000000.0f;
                    float mindxOther, mindyOther;
                    int minIndexOther=-1;

                    for (auto c = 0;c < g.teamMan.teams[otherTeam].creep.size();c++){
                        Creep & creep = g.teamMan.teams[otherTeam].creep[c];
                        
                        //Calc distance between generator and creep
                        float dx,dy;
                        dx = g.arenaMan.getGeneratorPosition(y,gi).x - creep.position.x;
                        dy = g.arenaMan.getGeneratorPosition(y,gi).y - creep.position.y;
                        float dist = sqrt(dx*dx+dy*dy);
                
                        if (dist < minDistOther){
                            minDistOther = dist;
                            minIndexOther = c;
                            mindxOther = dx;
                            mindyOther = dy;
                        }

                    }

                    for (auto p = 0;p < g.teamMan.teams[otherTeam].players.size();p++){
                        Player & player = g.teamMan.teams[otherTeam].players[p];
                        if (player.hasHost == true &&
                            player.tank.health > 0)
                        {                
                            //Calc distance between generator and player
                            float dx,dy;
                            dx = g.arenaMan.getGeneratorPosition(y,gi).x - player.tank.position.x;
                            dy = g.arenaMan.getGeneratorPosition(y,gi).y - player.tank.position.y;
                            float dist = sqrt(dx*dx+dy*dy);
                
                            if (dist < minDistOther){
                                minDistOther = dist;
                                minIndexOther = p;
                                mindxOther = dx;
                                mindyOther = dy;
                            }
                        }

                    }

                    // If closest opponent is closer than BASE_LASER_PROXIMAL
                    // fire at it!
                    if (minIndexOther != -1 && g.teamMan.teams[y].gen[gi].health > 0 && minDistOther <= DEATH_LASER_PROXIMAL)
                    {
                        g.teamMan.teams[y].gen[gi].prjctls.push_back(Projectile());
                    
                    
                        float mag = sqrt(mindxOther*mindxOther+mindyOther*mindyOther);
                        g.teamMan.teams[y].gen[gi].prjctls.back().velocity.x = -DEATH_LASER_SPEED*mindxOther/mag; 
                        g.teamMan.teams[y].gen[gi].prjctls.back().velocity.y = -DEATH_LASER_SPEED*mindyOther/mag;

                        g.teamMan.teams[y].gen[gi].prjctls.back().position.x = g.arenaMan.getGeneratorPosition(y,gi).x+0;
                        g.teamMan.teams[y].gen[gi].prjctls.back().position.y = g.arenaMan.getGeneratorPosition(y,gi).y+0;

                        g.teamMan.teams[y].gen[gi].prjctls.back().angle = atan2(mindyOther,mindxOther)*(180.0f/3.141567f)-90.0f;

                        g.teamMan.teams[y].gen[gi].prjctls.back().creationTime = accumulatingClock.getElapsedTime().asSeconds();
                        g.teamMan.teams[y].gen[gi].prjctls.back().damage = 1;
                    }
                }
            }
            //For each death laser segment, keep it moving. also, remove expired segments.
             for (int gi=0;gi < g.arenaMan.getGeneratorCount(y);gi++)
             {
                for (auto b = g.teamMan.teams[y].gen[gi].prjctls.begin();b != g.teamMan.teams[y].gen[gi].prjctls.end();)
                {
                    b->position.x = b->position.x + b->velocity.x*loopTime.asSeconds()*PIXELS_PER_SECOND;
                    b->position.y = b->position.y + b->velocity.y*loopTime.asSeconds()*PIXELS_PER_SECOND;
                    CollisionResult cr1, cr2;
                    sf::Vector2u sz(32,32); //= g.assetMan.getImage(ImageType::ProjectileHealRay).img.getSize();
                    bool yes1 = isTankCollision(b->position, sf::Vector2f((float)sz.x,(float)sz.y), g.teamMan.teams[otherTeam].players,cr1,otherTeam);
                    bool yes2 = isCreepCollision(b->position, sf::Vector2f((float)sz.x,(float)sz.y),g.teamMan.teams[otherTeam].creep,cr2,otherTeam);
                    if (yes1){
                        g.teamMan.teams[cr1.team].players[cr1.slot].tank.health -= b->damage;
                        if (g.teamMan.teams[cr1.team].players[cr1.slot].tank.health <= 0 )
                        {//Server must empower player
                            Explosion exp;
                            exp.type = ExplosionType::TankDeath;
                            exp.position = cr1.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);

                        }else if (g.teamMan.teams[cr1.team].players[cr1.slot].tank.health > 0)
                        {
                            Explosion exp;
                            exp.type = ExplosionType::TankHit;
                            exp.position = cr1.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);
                        }
                    }

                     if (yes2){
                        g.teamMan.teams[cr2.team].creep[cr2.slot].health -= 1;//b->damage;
                        if (g.teamMan.teams[cr2.team].creep[cr2.slot].health <= 0 )
                        {//Server must empower player
                            Explosion exp;
                            exp.type = ExplosionType::TankDeath;
                            exp.position = cr2.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);

                        }else if (g.teamMan.teams[cr2.team].creep[cr2.slot].health > 0)
                        {
                            Explosion exp;
                            exp.type = ExplosionType::TankHit;
                            exp.position = cr2.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);
                        }
                    }

                    if (yes1 || yes2 ||  b->creationTime + DEATH_LASER_LIFE_S < accumulatedClock)
                    {
                        b = g.teamMan.teams[y].gen[gi].prjctls.erase(b);
                    }else
                    {
                        b++;
                    }
                }
             }

            
            //Do Base Heal Ray
            if (healLaserClock.getElapsedTime().asSeconds() > HEAL_LASER_SPAWN_S)
            {
                float minDistUs = 400000000.0f;
                float mindxUs, mindyUs;
                int minIndexUs=-1;

                for (auto p = 0;p < g.teamMan.teams[y].players.size();p++){
                    Player & player = g.teamMan.teams[y].players[p];
                    if (player.hasHost == true)
                    {
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

                }

                // If closest ally is closer than BASE_LASER_PROXIMAL
                // fire at it!
                if (minIndexUs != -1 && 
                    g.teamMan.teams[y].base1.health > 0 && 
                    g.teamMan.teams[y].players[minIndexUs].tank.health < g.teamMan.teams[y].players[minIndexUs].tank.maxHealth && 
                    minDistUs <= HEAL_LASER_PROXIMAL)
                {
                    g.teamMan.teams[y].base1.prjctls.push_back(Projectile());
                    
                    
                    float mag = sqrt(mindxUs*mindxUs+mindyUs*mindyUs);
                    g.teamMan.teams[y].base1.prjctls.back().velocity.x = -HEAL_LASER_SPEED*mindxUs/mag; 
                    g.teamMan.teams[y].base1.prjctls.back().velocity.y = -HEAL_LASER_SPEED*mindyUs/mag;

                    g.teamMan.teams[y].base1.prjctls.back().position.x = basePos.x;
                    g.teamMan.teams[y].base1.prjctls.back().position.y = basePos.y;

                    g.teamMan.teams[y].base1.prjctls.back().angle = atan2(mindyUs,mindxUs)*(180.0f/3.141567f)-90.0f;

                    g.teamMan.teams[y].base1.prjctls.back().creationTime = accumulatingClock.getElapsedTime().asSeconds();
                    g.teamMan.teams[y].base1.prjctls.back().damage = 1;
                }
            }
            //For each heal laser segment, keep it moving. also, remove expired segments.
            for (auto b = g.teamMan.teams[y].base1.prjctls.begin();b != g.teamMan.teams[y].base1.prjctls.end();)
            {
                b->position.x = b->position.x + b->velocity.x*loopTime.asSeconds()*PIXELS_PER_SECOND;
                b->position.y = b->position.y + b->velocity.y*loopTime.asSeconds()*PIXELS_PER_SECOND;
                CollisionResult cr;
                sf::Vector2u sz(32,32) ;//= g.assetMan.getImage(ImageType::ProjectileHealRay).img.getSize();
                
                bool yes1 = isTankCollision(b->position, sf::Vector2f((float)sz.x,(float)sz.y), g.teamMan.teams[y].players,cr,y);
                if (yes1){
                    ////////
                    g.teamMan.teams[y].players[cr.slot].tank.health += b->damage;
                    ////////
                }
                if (yes1 || b->creationTime + HEAL_LASER_LIFE_S < accumulatedClock)
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
                    float avgPwr = 0;
                    for (auto ti = 0;ti < g.teamMan.teams[y].players.size();ti++){
                        avgPwr += g.teamMan.teams[y].players[ti].tank.power;
                    }
                    avgPwr /=g.teamMan.teams[y].players.size();

                    int r=0;
                    Creep newCreep;
                    newCreep.position = g.arenaMan.getStartPosition(y);

                    newCreep.lastWP = 0;
                    newCreep.creationTime = accumulatedClock;

                    if (y == 1){
                        if (rand()%2 == 0){
                            newCreep.creepType = ImageType::Minion1;
                            newCreep.wpType = (rand()%2) == 0 ? 0 : 2;
                            newCreep.health = 8 + avgPwr;///30;
                            newCreep.worth = 1;
                        }else{
                            newCreep.creepType = ImageType::Minion3;
                            newCreep.wpType = 1;
                            newCreep.health = 15 + avgPwr;///20;
                            newCreep.worth = 3;
                        }
                    }else if (y == 2){
                        if (rand()%2 == 0){
                            newCreep.creepType = ImageType::Minion2;
                            newCreep.wpType = (rand()%2) == 0 ? 0 : 2;
                            newCreep.health = 8 + avgPwr;///30;
                            newCreep.worth = 1;
                        }else{
                            newCreep.creepType = ImageType::Minion4;
                            newCreep.wpType = 1;
                            newCreep.health = 15 + avgPwr;///20;
                            newCreep.worth = 3;
                        }
                    }

                    /*if (y==1)
                        std::cout << y << ", " << newCreep.wpType << std::endl;;*/


                    ////randomly spawn to the right, or left. stay away from center.
                    ////Team 2 is 180 rotated, but the same.
                    //r = getNewCreepAngle1(y);

                    r = getNewCreepAngle2(y);

                    newCreep.angle = r-90.0f;
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
                bool yes1 = false;
                bool yes2 = false;
                
                    
                    ////Creep attacks base #5  priority
                    //{
                    //    float dx = c->position.x - g.arenaMan.getStartPosition(teami).x;
                    //    float dy = c->position.y - g.arenaMan.getStartPosition(teami).y;
                    //    
                    //    if (sqrt(dx*dx+dy*dy) < CREEP_BASE_PROXIMAL)
                    //    {
                    //        float ang =  atan2(dy,dx) / 0.0174531f;
                    //        c->angle = ang+90.0f;
                    //        c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                    //        c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                    //    }
                    //}


                    //Creep goes towards their waypoint - #4  priority
                    for (auto w = 0;w < g.arenaMan.getWaypoints(y).size();w++)
                    {//For each waypoint
                        int wayId = g.arenaMan.getWaypoints(y)[w].number;
                        int wayType = g.arenaMan.getWaypoints(y)[w].type;
                        /*if (y == 2)
                            std::cout << y << ", " << wayType << std::endl;*/
                        if (c->wpType == wayType && c->lastWP == wayId)
                        {
                            float dx = c->position.x - g.arenaMan.getWaypoints(y)[w].position.x;
                            float dy = c->position.y - g.arenaMan.getWaypoints(y)[w].position.y;
                            float dist = sqrt(dx*dx+dy*dy);
                            if (dist > CREEP_WAYPOINT_PROXIMAL)//  && c->creationTime +3.0f < accumulatedClock)
                            {
                                float ang =  atan2(dy,dx) / 0.0174531f;
                                c->angle = ang+90.0f;
                                c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                                c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                            }else{
                                c->lastWP++;
                            }
                        }
                   
                    }

                    //Creep attacks generator #3  priority
                    for (auto geni = 0;geni < g.arenaMan.getGeneratorCount(teami);geni++)
                    {
                        float dx = c->position.x - g.arenaMan.getGeneratorPosition(teami,geni).x+64;
                        float dy = c->position.y - g.arenaMan.getGeneratorPosition(teami,geni).y+64;
                    
                        if (sqrt(dx*dx+dy*dy) < CREEP_GENERATOR_PROXIMAL )
                        {
                            float ang =  atan2(dy,dx) / 0.0174531f;
                            c->angle = ang+90.0f;
                            c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                            c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                            break;
                        }
                    }

                    //Creep attacks tanks #2  priority               
                    for (auto tanki = 0;tanki < g.teamMan.teams[teami].players.size();tanki++)
                    {
                        if (g.teamMan.teams[teami].players[tanki].hasHost &&
                            g.teamMan.teams[teami].players[tanki].tank.health > 0)
                        {
                            //sf::FloatRect tankR(g.teamMan.teams[teami].players[tanki].tank.position,
                            float dx = c->position.x - g.teamMan.teams[teami].players[tanki].tank.position.x+12;
                            float dy = c->position.y - g.teamMan.teams[teami].players[tanki].tank.position.y+12;
                    
                    
                            if (sqrt(dx*dx+dy*dy) < CREEP_TANK_PROXIMAL)
                            {
                                float ang =  atan2(dy,dx) / 0.0174531f;
                                c->angle = ang+90.0f;
                                c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                                c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                                break;
                            }
                        }
                    
                    }

                    //Creep Attacks creep #1 priority
                    for (auto creepi = 0;creepi < g.teamMan.teams[teami].creep.size();creepi++)
                    {
                        //sf::FloatRect tankR(g.teamMan.teams[teami].players[tanki].tank.position,
                        float dx = c->position.x - g.teamMan.teams[teami].creep[creepi].position.x+32;
                        float dy = c->position.y - g.teamMan.teams[teami].creep[creepi].position.y+32;
                    
                        if (sqrt(dx*dx+dy*dy) < CREEP_CREEP_PROXIMAL)
                        {
                            float ang =  atan2(dy,dx) / 0.0174531f;
                            c->angle = ang+90.0f;
                            c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                            c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                            break;
                        }
                    }

                    ////Remove creep that has hit a tank or another creep
                    CollisionResult cr;
                    sf::Vector2u sz(32,32);// = g.assetMan.getImage(ImageType::Minion1).img.getSize();
               if (c->attackClock.getElapsedTime().asSeconds() > 1.0f)
                {
                        yes1 = isTankCollision(c->position, 
                                                    sf::Vector2f((float)sz.x,(float)sz.y),
                                                    g.teamMan.teams[otherTeam].players,
                                                    cr,
                                                    otherTeam);
                        sf::Vector2f tempPos1 = c->position;
                        tempPos1.x+=32;
                        tempPos1.y+=32;

                        
                        yes2 = isCreepCollision(tempPos1, 
                                                     sf::Vector2f((float)sz.x,(float)sz.y),
                                                     g.teamMan.teams[otherTeam].creep, 
                                                     cr,
                                                     otherTeam);
                
                    if (yes1){
                        g.teamMan.teams[cr.team].players[cr.slot].tank.health -= c->health;
                        if (g.teamMan.teams[cr.team].players[cr.slot].tank.health <= 0 )
                        {//Server must empower player
                            Explosion exp;
                            exp.type = ExplosionType::TankDeath;
                            exp.position = cr.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);

                        }else if (g.teamMan.teams[cr.team].players[cr.slot].tank.health > 0)
                        {
                            Explosion exp;
                            exp.type = ExplosionType::TankHit;
                            exp.position = cr.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);
                        }
                    }


                    if (yes2)
                    {
                        c->velocity =-c->velocity;
                        g.teamMan.teams[cr.team].creep[cr.slot].health -= c->health/2;
                        if (g.teamMan.teams[cr.team].creep[cr.slot].health <= 0 )
                        {//Server must empower player
                            Explosion exp;
                            exp.type = ExplosionType::TankDeath;
                            exp.position = cr.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);

                        }else if (g.teamMan.teams[cr.team].creep[cr.slot].health > 0)
                        {
                            Explosion exp;
                            exp.type = ExplosionType::TankHit;
                            exp.position = cr.loc;
                            exp.index = 0;
                            g.teamMan.explosions.push_back(exp);
                        }
                    }
                    c->attackClock.restart();
                }
                if (c->health <= 0 || yes1 || (yes2 && rand()%2==1) || c->creationTime + CREEP_LIFE_S < accumulatedClock )
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
        //sf::sleep(sf::milliseconds(0));
    }

    if (sendStateTimer.getElapsedTime().asMilliseconds() > SEND_STATE_MS && stateUpdated)
    {
        Messages::sendStateOfUnion(g.server, g.teamMan, g.arenaMan);
        sendStateTimer.restart();
    }
    return getSummary(0).a;
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, Game & g)
{

    return 0;
}



