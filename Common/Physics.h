#ifndef Physics_h_
#define Physics_h_

#include <SFML\System.hpp>
#include "Game.h"

using namespace tg;

bool isTankCollision(sf::Vector2f projPos, sf::Vector2f projSize, Game & g, sf::Int32 damage, sf::Uint32 team=-1)
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
                sf::FloatRect fr(tp->tank.position,sf::Vector2f(64,64));
                sf::FloatRect pr(projPos, projSize);
                if (fr.contains(projPos))
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

int isCreepCollision(sf::Vector2f projPos, sf::Vector2f projSize, Game & g, sf::Uint32 damage, sf::Uint32 team=-1)
{
    for (int y = 1; y < 3 ; y++)
    {
        //skip doing collision if a safe team is specified
        if (y != -1 && team == y)
            continue;
        
        for (auto c = g.teamMan.teams[y].creep.begin();c != g.teamMan.teams[y].creep.end();)
        {
            sf::FloatRect fr(c->position,sf::Vector2f(48,48));
            sf::FloatRect pr(projPos, projSize);
            if (fr.intersects(pr))//fr.contains(projPos))
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
        
        for (int gi =0;gi < g.arenaMan.getGeneratorCount(y);gi++)
        {
            sf::Vector2f genPos = g.arenaMan.getGeneratorPosition(y,gi);

            if (projPos.x > genPos.x &&
                projPos.x < genPos.x+125.0f &&
                projPos.y > genPos.y && 
                projPos.y < genPos.y+125.0f )
            {
            
                g.teamMan.teams[y].gen[gi].health-=damage;
                if (g.teamMan.teams[y].gen[gi].health<=0)
                    g.teamMan.teams[y].gen[gi].health=0;
                return true;
            }
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

bool MeAndMyTank___Bitch(Game & g, Player & player, sf::Uint32 playerTeam, sf::Time frameTime, sf::Clock & clock)
{
    const float PIXELS_PER_SECOND = 10.0f;
    //TODO: perhaps do this is little more similarly to the other notes recieved from client
    player.tank.velocity.x = player.tank.throttle * (float)cos(player.tank.bodyAngle * (0.0174531f));
    player.tank.velocity.y = player.tank.throttle * (float)sin(player.tank.bodyAngle * (0.0174531f));
                        
                        
    if (player.tank.attacking == AttackAction::Attacking)
    {
        player.prjctls.push_back(Projectile());
        //TODO: THINK: do you want projectile to inherit the tanks' velocity?
                    
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
        player.prjctls.back().damage = 1+player.tank.power/10.0f;

        player.tank.attacking = AttackAction::Idle;
    }
                       
    if (player.tank.accelerating == AccelerateAction::Forward)
    {
        player.tank.throttle+=2;
        if (player.tank.throttle > 56)
            player.tank.throttle = 56;
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
                        
    player.tank.turningBody = TurnAction::Idle;
    player.tank.accelerating = AccelerateAction::Idle;

    for (auto i = player.prjctls.begin();i != player.prjctls.end();)
    {
        float accTime = clock.getElapsedTime().asSeconds();
        if ( (i->creationTime + 0.75f)  < accTime )
        {
            //Remove projectiles who have run out of power.
            i = player.prjctls.erase(i);
        }else
        {
            i->position.x = i->position.x + i->velocity.x * frameTime.asSeconds()*PIXELS_PER_SECOND;
            i->position.y = i->position.y + i->velocity.y * frameTime.asSeconds()*PIXELS_PER_SECOND;
            i->totalDistance+=1;

            ////Remove projectile that has hit a tank, creep, generator, or base.
            sf::Vector2u sz = g.assetMan.getProjectileImage("Projectile").img->getSize();
            bool yes1 = isTankCollision(i->position, sf::Vector2f((float)sz.x,(float)sz.y), g,i->damage,playerTeam);
            int yes2 = isCreepCollision(i->position, sf::Vector2f((float)sz.x,(float)sz.y),g,i->damage,playerTeam);
            bool yes3 = isGeneratorCollision(i->position,g, i->damage,playerTeam);
            bool yes4 = isBaseCollision(i->position,g, i->damage,playerTeam);
                            
            if (yes1!=0 || yes2 || yes3 || yes4)
            {
                i = player.prjctls.erase(i);
                if (yes2==2){
                    player.tank.power++;
                    player.tank.maxHealth+=3;
                }
            }
            else
                i++;
        }
    }

    //TODO: HACK: hardcoded arena boundaries based on debug map and a 125px wide floor tile.
    if (player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND < (125) ||
        player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND > (59*125) )
    {
        player.tank.velocity.x = 0;
    }

    if (player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND < (125) ||
        player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND > (59*125) )
    {
        player.tank.velocity.y = 0;
    }

    player.tank.position.x = player.tank.position.x + player.tank.velocity.x *  frameTime.asSeconds()*PIXELS_PER_SECOND;
    player.tank.position.y = player.tank.position.y + player.tank.velocity.y *  frameTime.asSeconds()*PIXELS_PER_SECOND;

    sf::Vector2f basePos = g.arenaMan.getStartPosition(playerTeam);
    if (player.tank.position.x > basePos.x     &&
        player.tank.position.x < basePos.x+125 &&
        player.tank.position.y > basePos.y     &&
        player.tank.position.y < basePos.y+125)
    {
        //Tank is in base. 
        //TODO: Display upgrade dialog
    }
    return true;
}




#endif
