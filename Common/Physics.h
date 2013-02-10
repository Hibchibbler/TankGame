#ifndef Physics_h_
#define Physics_h_

#include <SFML\System.hpp>
#include "Game.h"

using namespace tg;

#define PIXELS_PER_SECOND 10.0f

//What was struck?
//And, Where was it?
class CollisionResult
{
public:
    sf::Uint32 team;
    sf::Uint32 slot;
    sf::Vector2f loc;
};

bool isTankCollision(sf::Vector2f projPos, sf::Vector2f projSize, std::vector<Player> & players, CollisionResult & cr, sf::Uint32 team=-1)
{
    ////TODO: assumes size of tank.
    for (int ti = 0;ti < players.size();ti++)
    {
        if (players[ti].hasHost)
        {
            Tank & tank = players[ti].tank;
            sf::FloatRect fr(tank.position,sf::Vector2f(64,64));
            sf::FloatRect pr(projPos, projSize);
            if (fr.intersects(pr))
            {
                cr.team = team;
                cr.slot = ti;
                cr.loc = projPos;
                return true;
            }
        }
    }
    return false;    
}

bool isCreepCollision(sf::Vector2f projPos, sf::Vector2f projSize, std::vector<Creep> & creeps, CollisionResult & cr, sf::Uint32 team=-1)
{
    ////TODO: assumes size of creep.
    for (int ci = 0;ci < creeps.size();ci++)
    {
        Creep & creep = creeps[ci];
        sf::FloatRect fr(creep.position,sf::Vector2f(48,48));
        sf::FloatRect pr(projPos, projSize);
        if (fr.intersects(pr))
        {
            cr.team = team;
            cr.slot = ci;
            cr.loc = projPos;
            return true;
        }
    }

    return false;
}

int isGeneratorCollision(sf::Vector2f projPos, sf::Vector2f projSize, std::vector<sf::Vector2f> & gens, CollisionResult & cr, sf::Uint32 team=-1)
{
    ////TODO: assumes size of generator.
    for (int gi =0;gi < gens.size();gi++)
    {
        sf::Vector2f genPos = gens[gi];

        if (projPos.x > genPos.x &&
            projPos.x < genPos.x+128.0f &&
            projPos.y > genPos.y && 
            projPos.y < genPos.y+128.0f )
        {
            
            cr.team = team;
            cr.slot = gi;
            cr.loc = projPos;
            return true;
        }
    }
    return false;
}
bool isBaseCollision(sf::Vector2f projPos, sf::Vector2f projSize, sf::Vector2f basePos, CollisionResult & cr, sf::Uint32 team=-1)
{
    ////TODO: assumes size of base.
    if (projPos.x > basePos.x &&
        projPos.x < basePos.x+128.0f &&
        projPos.y > basePos.y && 
        projPos.y < basePos.y+128.0f )
    {
        cr.team = team;
        cr.slot = -1;//Only one base per team.
        cr.loc = projPos;
        return true;
    }
    return false;
}

int doExplosiveStrike(std::list<Explosion> & explosions, sf::Int32 & health, sf::Vector2f strikeLoc, sf::Uint32 team, sf::Uint32 slot, sf::Uint32 damage)
{
    health -= damage;
    if (health <= 0 )
    {//Server must empower player
        Explosion exp;
        exp.type = ExplosionType::TankDeath;
        exp.position = strikeLoc;
        exp.index = 0;
        explosions.push_back(exp);
        return 2;
    }else if (health > 0)
    {
        Explosion exp;
        exp.type = ExplosionType::TankHit;
        exp.position = strikeLoc;
        exp.index = 0;
        explosions.push_back(exp);
        return 1;
    }
    return 0;
}

sf::Uint32 fireProjectile(Player & player, sf::Clock & clock)
{
    if (player.tank.attacking == AttackAction::Attacking &&
        player.tank.health > 0)
    {
        player.prjctls.push_back(Projectile());
        player.prjctls.back().velocity.x = 200.0f*((float)cos(player.tank.turretAngle*(0.0174531f))) + player.tank.velocity.x;
        player.prjctls.back().velocity.y = 200.0f*((float)sin(player.tank.turretAngle*(0.0174531f))) + player.tank.velocity.y;

        float vx,vy;
        vx =  player.prjctls.back().velocity.x;
        vy =  player.prjctls.back().velocity.y;

        player.prjctls.back().position.x = player.tank.position.x;// + vx;//(vx/75.0f)*5.5f;
        player.prjctls.back().position.y = player.tank.position.y;// + vy;//(vy/75.0f)*5.5f;
                    
        //std::cout << player.prjctls.back().position.x << ", " << player.prjctls.back().position.y << std::endl;
        player.prjctls.back().creationTime = clock.getElapsedTime().asSeconds();
        player.prjctls.back().angle = player.tank.turretAngle;
        player.prjctls.back().damage = 3+player.tank.power/10.0f;

        player.tank.attacking = AttackAction::Idle;
    }
    return 0;
}

sf::Uint32 updateVelocity(Player & player, std::vector<sf::Vector2f> & obstructionList, sf::Time frameTime)
{
    if (player.tank.accelerating == AccelerateAction::Forward)
    {
        player.tank.throttle+=2;
        float max = (13+player.tank.power/10.0f > 40 ? 40 : 13+player.tank.power/5.0f);
        if (player.tank.throttle > max)
            player.tank.throttle = max;
    }else if (player.tank.accelerating == AccelerateAction::Reverse)
    {
        player.tank.throttle-=2;
        if (player.tank.throttle < -8)
            player.tank.throttle = -8;
    }

    if (player.tank.turningBody == TurnAction::Right)
    {
        player.tank.bodyAngle += 10;
    }else if (player.tank.turningBody == TurnAction::Left)
    {
        player.tank.bodyAngle -= 10;
    }
    if (player.tank.bodyAngle >= 360.0f || player.tank.bodyAngle <= -360.0f ){
        player.tank.bodyAngle = 0;
    }

    //TODO: HACK: hardcoded arena boundaries based on debug map and a 128px wide floor tile.
    if (player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND < (128) ||
        player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND > (59*128) )
    {
        player.tank.velocity.x = 0;
    }

    if (player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND < (128) ||
        player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND > (59*128) )
    {
        player.tank.velocity.y = 0;
    }

    player.tank.turningBody = TurnAction::Idle;
    player.tank.accelerating = AccelerateAction::Idle;

    player.tank.velocity.x = player.tank.throttle * (float)cos(player.tank.bodyAngle * (0.0174531f));
    player.tank.velocity.y = player.tank.throttle * (float)sin(player.tank.bodyAngle * (0.0174531f));

    bool intersects = false;
    for (auto oi = obstructionList.begin();oi != obstructionList.end();oi++)
    {
        sf::FloatRect fr(oi->x,oi->y, 128,128);
        sf::Vector2f p(player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND,
                       player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND);
        if (fr.contains(p)){
            intersects = true;
            break;
        }
    }
    if (!intersects)
    {
        player.tank.position.x = player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND;
        player.tank.position.y = player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND;
    }
   
    //player.tank.position.x = floor(player.tank.position.x);
    //player.tank.position.y = floor(player.tank.position.y);
    return 0;
}

bool updateProjectilCollisions(Game & g, Player & player, sf::Uint32 playerTeam, std::vector<sf::Vector2f> & obstructionList, bool client, sf::Time frameTime, sf::Clock & clock)
{
    for (auto i = player.prjctls.begin();i != player.prjctls.end();)
    {
        float accTime = clock.getElapsedTime().asSeconds();

        bool obstructed = false;
        sf::Vector2f p(i->position.x + i->velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND,
                       i->position.y + i->velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND);
        
        
        for (auto oi = obstructionList.begin();oi != obstructionList.end();oi++)
        {
            sf::FloatRect fr(oi->x,oi->y, 128,128);
            if (fr.contains(p)){
                obstructed = true;
                break;
            }
        }
        
        
        
        ////Remove projectile that has hit a tank, creep, generator, or base.
        sf::Uint32 damage = (client ? 0 : i->damage);
        sf::Vector2u sz(32,32);
        sf::Uint32 otherTeam  = (playerTeam==1 ? 2 : 1);
        CollisionResult cr1;
        CollisionResult cr2;
        CollisionResult cr3;
        CollisionResult cr4;

        bool yes1 = isTankCollision(i->position, 
                                    sf::Vector2f((float)sz.x,(float)sz.y),
                                    g.teamMan.teams[otherTeam].players,
                                    cr1,
                                    otherTeam);

        bool yes2 = isCreepCollision(i->position, 
                                    sf::Vector2f((float)sz.x,(float)sz.y),
                                    g.teamMan.teams[otherTeam].creep, 
                                    cr2,
                                    otherTeam);

        bool yes3 = isGeneratorCollision(i->position,
                                    sf::Vector2f((float)sz.x,(float)sz.y) ,
                                    g.arenaMan.getGenerator(otherTeam),
                                    cr3,
                                    otherTeam);

        bool yes4 = isBaseCollision(i->position,
                                    sf::Vector2f((float)sz.x,(float)sz.y),
                                    g.arenaMan.getStartPosition(otherTeam),//g.teamMan.teams[otherTeam].base1,
                                    cr4,
                                    otherTeam);


        if (yes1 || yes2 || yes3 || yes4 || obstructed || (i->creationTime + 0.56f)  < accTime )
        {
            //Remove projectiles who have run their course..
            Explosion exp;
                exp.type = ExplosionType::None;
                exp.position = i->position;
                exp.index=0;
                
                if (yes1){
                    int ret = doExplosiveStrike(g.teamMan.explosions,
                                                g.teamMan.teams[cr1.team].players[cr1.slot].tank.health,
                                                cr1.loc,
                                                cr1.team,
                                                cr1.slot,
                                                damage);
                    g.teamMan.teams[cr1.team].players[cr1.slot].tank.health -= damage;
                    if (g.teamMan.teams[cr1.team].players[cr1.slot].tank.health <= 0 )
                    {//Server must empower player
                        Explosion exp;
                        exp.type = ExplosionType::TankDeath;
                        exp.position = i->position;
                        exp.index = 0;
                        g.teamMan.explosions.push_back(exp);
                    }else if (g.teamMan.teams[cr1.team].players[cr1.slot].tank.health > 0)
                    {
                        Explosion exp;
                        exp.type = ExplosionType::TankHit;
                        exp.position = i->position;
                        exp.index = 0;
                        g.teamMan.explosions.push_back(exp);
                    }
                    
                }

                if (yes2){
                    int ret = doExplosiveStrike(g.teamMan.explosions,
                                                g.teamMan.teams[cr2.team].creep[cr2.slot].health,
                                                cr2.loc,
                                                cr2.team,
                                                cr2.slot,
                                                damage);
                    if (ret == 2){
                        player.tank.power+=1;
                        player.tank.maxHealth+=2;
                    }

                }

                if (yes3){
                    int ret = doExplosiveStrike(g.teamMan.explosions,
                                                g.teamMan.teams[cr3.team].gen[cr3.slot].health,
                                                cr3.loc,
                                                cr3.team,
                                                cr3.slot,
                                                damage);
                    if (ret == 2){
                        
                    }
              
                }
                
                if (yes4){
                    int ret = doExplosiveStrike(g.teamMan.explosions,
                                                g.teamMan.teams[cr4.team].base1.health,
                                                cr4.loc,
                                                cr4.team,
                                                cr4.slot,
                                                damage);
                    if (ret == 2){
                        
                    }
                }
            i = player.prjctls.erase(i);
        }else //It keeps going...
        {
            i->position.x = i->position.x + i->velocity.x * frameTime.asSeconds()*PIXELS_PER_SECOND;
            i->position.y = i->position.y + i->velocity.y * frameTime.asSeconds()*PIXELS_PER_SECOND;
            i->totalDistance+=1;
            i++;
        }
    }
    return true;
}


std::vector<sf::Vector2f> prepareObstructionList(ArenaManager & arenaMan)
{
    std::vector<sf::Vector2f> obstructionList;
    for (int i = 0;i < arenaMan.getMapHorizTileNum()*arenaMan.getMapVertTileNum();i++){
        Tile &tile = arenaMan.getTile(i);
        if (tile.getId() == 4)
            obstructionList.push_back(tile.getPosition());
    }
    return obstructionList;
}



#endif
