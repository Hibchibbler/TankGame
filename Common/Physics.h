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
    enum Type{
        Tank,
        Creep,
        Base,
        Turret
    };
    sf::Uint32 type;
    sf::Uint32 team;
    sf::Uint32 slot;
    sf::Vector2f loc;
    sf::Uint32 damage;
};

#define CENTER sf::Vector2i( 0, 0)
#define NIL    sf::Vector2i( 0, 0)
//
sf::Vector2i fogMask0[] = 
{
            NIL        ,         NIL        ,         NIL        ,        NIL         ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        ,         NIL        , sf::Vector2i(-1,-4), sf::Vector2i( 0,-4), sf::Vector2i( 1,-4),         NIL        ,         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-2,-3), sf::Vector2i(-1,-3), sf::Vector2i( 0,-3), sf::Vector2i( 1,-3), sf::Vector2i( 2,-3),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-2,-2), sf::Vector2i(-1,-2), sf::Vector2i( 0,-2), sf::Vector2i( 1,-2), sf::Vector2i( 2,-2),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-3,-1), sf::Vector2i(-2,-1), sf::Vector2i(-1,-1), sf::Vector2i( 0,-1), sf::Vector2i( 1,-1), sf::Vector2i( 2,-1), sf::Vector2i( 3,-1),         NIL        ,         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-3, 0), sf::Vector2i(-2, 0), sf::Vector2i(-1, 0),        CENTER      , sf::Vector2i( 1, 0), sf::Vector2i( 2, 0), sf::Vector2i( 3, 0),         NIL        ,         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-3, 1), sf::Vector2i(-2, 1), sf::Vector2i(-1, 1), sf::Vector2i( 0, 1), sf::Vector2i( 1, 1), sf::Vector2i( 2, 1), sf::Vector2i( 3, 1),         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-2, 2), sf::Vector2i(-1, 2), sf::Vector2i( 0, 2), sf::Vector2i( 1, 2), sf::Vector2i( 2, 2),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-2, 3), sf::Vector2i(-1, 3), sf::Vector2i( 0, 3), sf::Vector2i( 1, 3), sf::Vector2i( 2, 3),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        ,         NIL        , sf::Vector2i(-1, 4), sf::Vector2i( 0, 4), sf::Vector2i( 1, 4),         NIL        ,         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        ,        NIL         ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        
};

sf::Vector2i fogMask1[] = 
{
            NIL        ,         NIL        ,         NIL        ,        NIL         , sf::Vector2i(-1,-5), sf::Vector2i( 0,-5), sf::Vector2i( 1,-5),         NIL        ,         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-2,-4), sf::Vector2i(-1,-4), sf::Vector2i( 0,-4), sf::Vector2i( 1,-4), sf::Vector2i( 2,-4),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-3,-3), sf::Vector2i(-2,-3), sf::Vector2i(-1,-3), sf::Vector2i( 0,-3), sf::Vector2i( 1,-3), sf::Vector2i( 2,-3), sf::Vector2i( 3,-3),         NIL        ,         NIL        ,
            NIL        , sf::Vector2i(-4,-2), sf::Vector2i(-3,-2), sf::Vector2i(-2,-2), sf::Vector2i(-1,-2), sf::Vector2i( 0,-2), sf::Vector2i( 1,-2), sf::Vector2i( 2,-2), sf::Vector2i( 3,-2), sf::Vector2i( 4,-2),         NIL        ,
    sf::Vector2i(-5,-1), sf::Vector2i(-4,-1), sf::Vector2i(-3,-1), sf::Vector2i(-2,-1), sf::Vector2i(-1,-1), sf::Vector2i( 0,-1), sf::Vector2i( 1,-1), sf::Vector2i( 2,-1), sf::Vector2i( 3,-1), sf::Vector2i( 4,-1), sf::Vector2i( 5,-1),
    sf::Vector2i(-5, 0), sf::Vector2i(-4, 0), sf::Vector2i(-3, 0), sf::Vector2i(-2, 0), sf::Vector2i(-1, 0),        CENTER      , sf::Vector2i( 1, 0), sf::Vector2i( 2, 0), sf::Vector2i( 3, 0), sf::Vector2i( 4, 0), sf::Vector2i( 5, 0),
    sf::Vector2i(-5, 1), sf::Vector2i(-4, 1), sf::Vector2i(-3, 1), sf::Vector2i(-2, 1), sf::Vector2i(-1, 1), sf::Vector2i( 0, 1), sf::Vector2i( 1, 1), sf::Vector2i( 2, 1), sf::Vector2i( 3, 1), sf::Vector2i( 4, 1), sf::Vector2i( 5, 1),
            NIL        , sf::Vector2i(-4, 2), sf::Vector2i(-3, 2), sf::Vector2i(-2, 2), sf::Vector2i(-1, 2), sf::Vector2i( 0, 2), sf::Vector2i( 1, 2), sf::Vector2i( 2, 2), sf::Vector2i( 3, 2), sf::Vector2i( 4, 2),         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-3, 3), sf::Vector2i(-2, 3), sf::Vector2i(-1, 3), sf::Vector2i( 0, 3), sf::Vector2i( 1, 3), sf::Vector2i( 2, 3), sf::Vector2i( 3, 3),         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-2, 4), sf::Vector2i(-1, 4), sf::Vector2i( 0, 4), sf::Vector2i( 1, 4), sf::Vector2i( 2, 4),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        ,        NIL         , sf::Vector2i(-1, 5), sf::Vector2i( 0, 5), sf::Vector2i( 1, 5),         NIL        ,         NIL        ,         NIL        ,         NIL        
};
//11*11 elements

sf::Vector2i fogMask2[] = 
{
            NIL        ,         NIL        ,         NIL        ,         NIL        ,        NIL         ,        NIL         , sf::Vector2i(-1,-7), sf::Vector2i( 0,-7), sf::Vector2i( 1,-7),        NIL         ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        ,         NIL        ,        NIL         , sf::Vector2i(-2,-6), sf::Vector2i(-1,-6), sf::Vector2i( 0,-6), sf::Vector2i( 1,-6), sf::Vector2i( 2,-6),         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-4,-5), sf::Vector2i(-3,-5), sf::Vector2i(-2,-5), sf::Vector2i(-1,-5), sf::Vector2i( 0,-5), sf::Vector2i( 1,-5), sf::Vector2i( 2,-5), sf::Vector2i( 3,-5), sf::Vector2i( 4,-5),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-5,-4), sf::Vector2i(-4,-4), sf::Vector2i(-3,-4), sf::Vector2i(-2,-4), sf::Vector2i(-1,-4), sf::Vector2i( 0,-4), sf::Vector2i( 1,-4), sf::Vector2i( 2,-4), sf::Vector2i( 3,-4), sf::Vector2i( 4,-4), sf::Vector2i( 5,-4),         NIL        ,         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-5,-3), sf::Vector2i(-4,-3), sf::Vector2i(-3,-3), sf::Vector2i(-2,-3), sf::Vector2i(-1,-3), sf::Vector2i( 0,-3), sf::Vector2i( 1,-3), sf::Vector2i( 2,-3), sf::Vector2i( 3,-3), sf::Vector2i( 4,-3), sf::Vector2i( 5,-3),         NIL        ,         NIL        ,
            NIL        , sf::Vector2i(-6,-2), sf::Vector2i(-5,-2), sf::Vector2i(-4,-2), sf::Vector2i(-3,-2), sf::Vector2i(-2,-2), sf::Vector2i(-1,-2), sf::Vector2i( 0,-2), sf::Vector2i( 1,-2), sf::Vector2i( 2,-2), sf::Vector2i( 3,-2), sf::Vector2i( 4,-2), sf::Vector2i( 5,-2), sf::Vector2i( 6,-2),         NIL        ,
    sf::Vector2i(-7,-1), sf::Vector2i(-6,-1), sf::Vector2i(-5,-1), sf::Vector2i(-4,-1), sf::Vector2i(-3,-1), sf::Vector2i(-2,-1), sf::Vector2i(-1,-1), sf::Vector2i( 0,-1), sf::Vector2i( 1,-1), sf::Vector2i( 2,-1), sf::Vector2i( 3,-1), sf::Vector2i( 4,-1), sf::Vector2i( 5,-1), sf::Vector2i( 6,-1), sf::Vector2i( 7,-1),
    sf::Vector2i(-7, 0), sf::Vector2i(-6, 0), sf::Vector2i(-5, 0), sf::Vector2i(-4, 0), sf::Vector2i(-3, 0), sf::Vector2i(-2, 0), sf::Vector2i(-1, 0),        CENTER      , sf::Vector2i( 1, 0), sf::Vector2i( 2, 0), sf::Vector2i( 3, 0), sf::Vector2i( 4, 0), sf::Vector2i( 5, 0), sf::Vector2i( 6, 0), sf::Vector2i( 7, 0),
    sf::Vector2i(-7, 1), sf::Vector2i(-6, 1), sf::Vector2i(-5, 1), sf::Vector2i(-4, 1), sf::Vector2i(-3, 1), sf::Vector2i(-2, 1), sf::Vector2i(-1, 1), sf::Vector2i( 0, 1), sf::Vector2i( 1, 1), sf::Vector2i( 2, 1), sf::Vector2i( 3, 1), sf::Vector2i( 4, 1), sf::Vector2i( 5, 1), sf::Vector2i( 6, 1), sf::Vector2i( 7, 1),
            NIL        , sf::Vector2i(-6, 2), sf::Vector2i(-5, 2), sf::Vector2i(-4, 2), sf::Vector2i(-3, 2), sf::Vector2i(-2, 2), sf::Vector2i(-1, 2), sf::Vector2i( 0, 2), sf::Vector2i( 1, 2), sf::Vector2i( 2, 2), sf::Vector2i( 3, 2), sf::Vector2i( 4, 2), sf::Vector2i( 5, 2), sf::Vector2i( 6, 2),         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-5, 3), sf::Vector2i(-4, 3), sf::Vector2i(-3, 3), sf::Vector2i(-2, 3), sf::Vector2i(-1, 3), sf::Vector2i( 0, 3), sf::Vector2i( 1, 3), sf::Vector2i( 2, 3), sf::Vector2i( 3, 3), sf::Vector2i( 4, 3), sf::Vector2i( 5, 3),         NIL        ,         NIL        ,
            NIL        ,         NIL        , sf::Vector2i(-5, 4), sf::Vector2i(-4, 4), sf::Vector2i(-3, 4), sf::Vector2i(-2, 4), sf::Vector2i(-1, 4), sf::Vector2i( 0, 4), sf::Vector2i( 1, 4), sf::Vector2i( 2, 4), sf::Vector2i( 3, 4), sf::Vector2i( 4, 4), sf::Vector2i( 5, 4),         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        , sf::Vector2i(-4, 5), sf::Vector2i(-3, 5), sf::Vector2i(-2, 5), sf::Vector2i(-1, 5), sf::Vector2i( 0, 5), sf::Vector2i( 1, 5), sf::Vector2i( 2, 5), sf::Vector2i( 3, 5), sf::Vector2i( 4, 5),         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        ,         NIL        ,        NIL         , sf::Vector2i(-2, 6), sf::Vector2i(-1, 6), sf::Vector2i( 0, 6), sf::Vector2i( 1, 6), sf::Vector2i( 2, 6),         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,
            NIL        ,         NIL        ,         NIL        ,         NIL        ,        NIL         ,        NIL         , sf::Vector2i(-1, 7), sf::Vector2i( 0, 7), sf::Vector2i( 1, 7),        NIL         ,         NIL        ,         NIL        ,         NIL        ,         NIL        ,         NIL        
};
//225

void partFogQuick(sf::Vector2f pos, Game & g, sf::Vector2i fogMask[], int num)
{
    int index;
    sf::Vector2f p;
    
    //We skip zeros...for efficiency, but we can't skip the Zero
    //that is implied by the tanks CURRENT location
    g.arenaMan.posToIndex(pos,index);
    if (index >= 0 && index < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum())
    {
        g.arenaMan.getTile(index).fog = false;
        g.arenaMan.getTile(index).fogFalseIndex = 0;
        g.arenaMan.getTile(index).fogClock.restart();
    }

    for (int i = 0;i < num;i++)
    {
        if (fogMask[i].x == 0 && fogMask[i].y == 0)
            continue;
        p.x = pos.x + 128*fogMask[i].x;
        p.y = pos.y + 128*fogMask[i].y;
        g.arenaMan.posToIndex(p,index);
        if (index >= 0 && index < g.arenaMan.getMapHorizTileNum()*g.arenaMan.getMapVertTileNum())
        {
            g.arenaMan.getTile(index).fog = false;
            g.arenaMan.getTile(index).fogFalseIndex = 0;
            g.arenaMan.getTile(index).fogClock.restart();
        }

    }
}

//void partFog(Tile & t)
//{
//    t.fog = false;
//    t.fogFalseIndex = 0;//It is present, and diminishing
//    t.fogClock.restart();
//}



bool isTankCollision(sf::Vector2f projPos, sf::Vector2f projSize, std::vector<Player> & players, CollisionResult & cr, sf::Uint32 team=-1)
{
    ////TODO: assumes size of tank.
    for (int ti = 0;ti < players.size();ti++)
    {
        if (players[ti].hasHost)
        {
            Tank & tank = players[ti].tank;
            sf::FloatRect fr(tank.position,sf::Vector2f(75,75));
            sf::FloatRect pr(projPos, projSize);
            if (fr.intersects(pr))
            {
                cr.team = team;
                cr.slot = ti;
                cr.loc = projPos;
                cr.type = CollisionResult::Tank;
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
        //sf::Vector2f tempPos;
        //tempPos = creep.position;
        ////tempPos.x += 32;
        ////tempPos.y += 32;
        sf::FloatRect fr(creep.position,sf::Vector2f(64,64));
        sf::FloatRect pr(projPos, projSize);
        if (fr.intersects(pr))
        {
            cr.team = team;
            cr.slot = ci;
            cr.loc = projPos;
            cr.type = CollisionResult::Creep;
            return true;
        }
    }

    return false;
}

int isGeneratorCollision(sf::Vector2f projPos, sf::Vector2f projSize, std::vector<TurretEntity> & gens, CollisionResult & cr, sf::Uint32 team=-1)
{
    ////TODO: assumes size of generator.
    for (int gi =0;gi < gens.size();gi++)
    {
        sf::Vector2f genPos = gens[gi].position;

        if (projPos.x > genPos.x &&
            projPos.x < genPos.x+128.0f &&
            projPos.y > genPos.y && 
            projPos.y < genPos.y+128.0f )
        {
            
            cr.team = team;
            cr.slot = gi;
            cr.loc = projPos;
            cr.type = CollisionResult::Turret;
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
        cr.type = CollisionResult::Base;
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
        float max = floor((13+player.tank.power/10.0f > 40 ? 40 : 13+player.tank.power/5.0f));
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

    ////TODO: HACK: hardcoded arena boundaries based on debug map and a 128px wide floor tile.
    //if (player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND < (128) ||
    //    player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND > (59*128) )
    //{
    //    player.tank.velocity.x = 0;
    //}

    //if (player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND < (128) ||
    //    player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND > (59*128) )
    //{
    //    player.tank.velocity.y = 0;
    //}

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

                Explosion exp;
                exp.type = ExplosionType::TankHit;
                exp.position = i->position;
                exp.index = 0;
                g.teamMan.explosions.push_back(exp);
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

        int yes1 = isTankCollision(i->position, 
                                    sf::Vector2f((float)sz.x,(float)sz.y),
                                    g.teamMan.teams[otherTeam].players,
                                    cr1,
                                    otherTeam);

        int yes2 = isCreepCollision(i->position, 
                                    sf::Vector2f((float)sz.x,(float)sz.y),
                                    g.teamMan.teams[otherTeam].creep, 
                                    cr2,
                                    otherTeam);

        int yes3 = isGeneratorCollision(i->position,
                                    sf::Vector2f((float)sz.x,(float)sz.y) ,
                                    g.arenaMan.getGenerator(otherTeam),
                                    cr3,
                                    otherTeam);

        int yes4 = isBaseCollision(i->position,
                                    sf::Vector2f((float)sz.x,(float)sz.y),
                                    g.arenaMan.getStartPosition(otherTeam),
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
                        player.tank.power+=g.teamMan.teams[cr2.team].creep[cr2.slot].worth;
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
