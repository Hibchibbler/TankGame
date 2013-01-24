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
#define CREEP_SPAWN_MS 1200
#define UPDATE_STATE_MS 50
#define SEND_STATE_MS 100//110
#define PIXELS_PER_SECOND 10
#define CREEP_LIFE_S 75.0f
#define HEAL_LASER_PROXIMAL 450.0f
#define DEATH_LASER_PROXIMAL 1100.0f
#define HEAL_LASER_LIFE_S 0.65f
#define DEATH_LASER_LIFE_S 0.80f
#define HEAL_LASER_SPEED 55.0f
#define DEATH_LASER_SPEED 125.0f
#define HEAL_LASER_SPAWN_S 0.07f
#define DEATH_LASER_SPAWN_S 0.006f
#define CREEP_TANK_PROXIMAL 800
#define CREEP_CREEP_PROXIMAL 700
//BASE_LASER_LIFE_S
static int RANDOM_LAG =0;
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
                        MeAndMyTank___Bitch(g,*pi, y,false,loopTime, accumulatingClock);
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

                    for (auto p = 0;p < g.teamMan.teams[otherTeam].players.size();p++){
                        Player & player = g.teamMan.teams[otherTeam].players[p];
                        if (player.hasHost == false)
                            continue;

                
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

                    // If closest opponent is closer than BASE_LASER_PROXIMAL
                    // fire at it!
                    if (minIndexOther != -1 && g.teamMan.teams[y].gen[gi].health > 0 && minDistOther <= DEATH_LASER_PROXIMAL)
                    {
                        g.teamMan.teams[y].gen[gi].prjctls.push_back(Projectile());
                    
                    
                        float mag = sqrt(mindxOther*mindxOther+mindyOther*mindyOther);
                        g.teamMan.teams[y].gen[gi].prjctls.back().velocity.x = -DEATH_LASER_SPEED*mindxOther/mag; 
                        g.teamMan.teams[y].gen[gi].prjctls.back().velocity.y = -DEATH_LASER_SPEED*mindyOther/mag;

                        g.teamMan.teams[y].gen[gi].prjctls.back().position.x = g.arenaMan.getGeneratorPosition(y,gi).x+32;
                        g.teamMan.teams[y].gen[gi].prjctls.back().position.y = g.arenaMan.getGeneratorPosition(y,gi).y+32;

                        g.teamMan.teams[y].gen[gi].prjctls.back().angle = atan2(mindyOther,mindxOther)*(180.0f/3.141567f)-90.0f;

                        g.teamMan.teams[y].gen[gi].prjctls.back().creationTime = accumulatingClock.getElapsedTime().asSeconds();
                        g.teamMan.teams[y].gen[gi].prjctls.back().damage = 1;
                    }
                }
            }
            //For each death laser segment, keep it moving. also, remove expired segments.
             for (int gi=0;gi < 2;gi++)
             {
                for (auto b = g.teamMan.teams[y].gen[gi].prjctls.begin();b != g.teamMan.teams[y].gen[gi].prjctls.end();)
                {
                    b->position.x = b->position.x + b->velocity.x*loopTime.asSeconds()*PIXELS_PER_SECOND;
                    b->position.y = b->position.y + b->velocity.y*loopTime.asSeconds()*PIXELS_PER_SECOND;
                    CollisionResult cr1;
                    sf::Vector2u sz = g.assetMan.getImage(ImageType::ProjectileHealRay).img.getSize();
                    bool yes1 = isTankCollision(b->position, sf::Vector2f((float)sz.x,(float)sz.y), g.teamMan.teams[otherTeam].players,cr1,otherTeam);
                    if (yes1){
                        int ret = doExplosiveStrike(g.teamMan.explosions,
                                                g.teamMan.teams[cr1.team].players[cr1.slot].tank.health,
                                                cr1.loc,
                                                cr1.team,
                                                cr1.slot,
                                                b->damage);
                    }

                    if (yes1 || b->creationTime + DEATH_LASER_LIFE_S < accumulatedClock)
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
                sf::Vector2u sz = g.assetMan.getImage(ImageType::ProjectileHealRay).img.getSize();
                
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

                //If creep is near opponent tank, or opponent creep..
                //go towards it.
                int teami = (y==1?2:1);
                for (auto tanki = 0;tanki < g.teamMan.teams[teami].players.size();tanki++)
                {
                    if (g.teamMan.teams[teami].players[tanki].hasHost)
                    {
                        //sf::FloatRect tankR(g.teamMan.teams[teami].players[tanki].tank.position,
                        float dx = c->position.x - g.teamMan.teams[teami].players[tanki].tank.position.x;
                        float dy = c->position.y - g.teamMan.teams[teami].players[tanki].tank.position.y;
                    
                    
                        if (sqrt(dx*dx+dy*dy) < CREEP_TANK_PROXIMAL)
                        {
                            float ang =  atan2(dy,dx) / 0.0174531f;
                            c->angle = ang+90.0f;
                            c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                            c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                        
                        }
                    }
                    
                }
                for (auto creepi = 0;creepi < g.teamMan.teams[teami].creep.size();creepi++)
                {
                    //sf::FloatRect tankR(g.teamMan.teams[teami].players[tanki].tank.position,
                    float dx = c->position.x - g.teamMan.teams[teami].creep[creepi].position.x;
                    float dy = c->position.y - g.teamMan.teams[teami].creep[creepi].position.y;
                    
                    if (sqrt(dx*dx+dy*dy) < CREEP_CREEP_PROXIMAL)
                    {
                        float ang =  atan2(dy,dx) / 0.0174531f;
                        c->angle = ang+90.0f;
                        c->velocity.x = -CREEP_SPEED*cos(ang * (0.0174531f));
                        c->velocity.y = -CREEP_SPEED*sin(ang * (0.0174531f));
                    }
                }

                ////Remove creep that is hit a tank or another creep
                CollisionResult cr;
                sf::Vector2u sz = g.assetMan.getImage(ImageType::Minion1).img.getSize();
                
                bool yes1 = isTankCollision(c->position, 
                                            sf::Vector2f((float)sz.x,(float)sz.y),
                                            g.teamMan.teams[otherTeam].players,
                                            cr,
                                            otherTeam);

                bool yes2 = isCreepCollision(c->position, 
                                             sf::Vector2f((float)sz.x,(float)sz.y),
                                             g.teamMan.teams[otherTeam].creep, 
                                             cr,
                                             otherTeam);
                if (yes1){
                    g.teamMan.teams[cr.team].players[cr.slot].tank.health -= 5;
                    if (!client && g.teamMan.teams[cr.team].players[cr.slot].tank.health <= 0 )
                    {//Server must empower player
                        Explosion exp;
                        exp.type = ExplosionType::TankDeath;
                        exp.position = cr.loc;
                        exp.index = 0;
                        g.teamMan.explosions.push_back(exp);

                    }else if (client && g.teamMan.teams[cr.team].players[cr.slot].tank.health <= 0)
                    {//client just gotta make explosions
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
                    g.teamMan.teams[cr.team].creep[cr.slot].health -= 5;
                    if (!client && g.teamMan.teams[cr.team].creep[cr.slot].health <= 0 )
                    {//Server must empower player
                        Explosion exp;
                        exp.type = ExplosionType::TankDeath;
                        exp.position = cr.loc;
                        exp.index = 0;
                        g.teamMan.explosions.push_back(exp);

                    }else if (client && g.teamMan.teams[cr.team].creep[cr.slot].health <= 0)
                    {//client just gotta make explosions
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



