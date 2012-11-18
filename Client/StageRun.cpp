#include "StageRun.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"

using namespace tg;

StageRun::StageRun()
    : GameStage()
{

}

sf::Uint32 StageRun::doRemoteEvent(TeamManager & teamMan, 
                            ArenaManager & arenaMan,
                            CommEvent & cevent,
                            sf::Uint32 connId,
                            sf::Uint32 msgId)
{
    return 0;
}

sf::Uint32 StageRun::doWindowEvent(sf::RenderWindow & w, 
                                          sf::Event & event)
{
    if (event.type == sf::Event::LostFocus){
        hasFocus = false;
    }else if (event.type == sf::Event::GainedFocus){
        hasFocus = true;
    }
    return 0;
}

sf::Uint32 StageRun::doLoop(Comm & comm, TeamManager & teamMan)
{
    sf::Uint32 scrWidth = getInput(0);
    sf::Uint32 scrHeight = getInput(1);
    sf::Uint32 myCID = getInput(2);
    sf::Uint32 myTeam = getInput(3);
    sf::Uint32 mySlot = getInput(4);
    
    Player & p = teamMan.getPlayerBySlot(myTeam,mySlot);
    switch (p.state){
        case PlayerState::Ready:
            p.state = PlayerState::Running;
            break;
        case PlayerState::SendingAction:
            if (lastActionReg & 0x1)
                Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,PlayerAction::ThrottleUp);
            if (lastActionReg & 0x2)
                Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,PlayerAction::ThrottleDown);
            if (lastActionReg & 0x4)
                Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,PlayerAction::BodyLeft);
            if (lastActionReg & 0x8)
                Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,PlayerAction::BodyRight);
            if (lastActionReg & 0x10)
                Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,PlayerAction::TurretMove);
            if (lastActionReg & 0x20)
                Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,PlayerAction::Attack);
            p.state = PlayerState::Running;
            break;

    }
    return getSummary(0);
}
sf::Uint32 StageRun::doLocalInput(sf::RenderWindow & window, TeamManager & teamMan)
{
    sf::Uint32 scrWidth = getInput(0);
    sf::Uint32 scrHeight = getInput(1);
    sf::Uint32 myCID = getInput(2);
    sf::Uint32 myTeam = getInput(3);
    sf::Uint32 mySlot = getInput(4);


    bool control = false;
    lastActionReg = 0;
    //We poll keyboard instead of relying on bios keyboard rate
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        teamMan.teams[myTeam].players[mySlot].tank.throttle += 1;
        if (teamMan.teams[myTeam].players[mySlot].tank.throttle > 25)
            teamMan.teams[myTeam].players[mySlot].tank.throttle = 25;  
        control = true;
        lastActionReg = lastActionReg | 0x00000001;
    }
    

    //max speed lower in reverse
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        teamMan.teams[myTeam].players[mySlot].tank.throttle -= 1;
        if (teamMan.teams[myTeam].players[mySlot].tank.throttle < -10)
            teamMan.teams[myTeam].players[mySlot].tank.throttle = -10;  
        control = true;
        lastActionReg = lastActionReg | 0x00000002;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        //Rotating left
        teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = teamMan.teams[myTeam].players[mySlot].tank.bodyAngle -  5;
        if (teamMan.teams[myTeam].players[mySlot].tank.bodyAngle >= 360.0f || teamMan.teams[myTeam].players[mySlot].tank.bodyAngle <= -360.0f ){
            teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = 0;
        }
        control = true;
        lastActionReg = lastActionReg | 0x00000004;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        //Rotating right
        teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = teamMan.teams[myTeam].players[mySlot].tank.bodyAngle +  5;
        if (teamMan.teams[myTeam].players[mySlot].tank.bodyAngle >= 360.0f || teamMan.teams[myTeam].players[mySlot].tank.bodyAngle <= -360.0f ){
            teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = 0;
        }
        lastActionReg = lastActionReg | 0x00000008;
    }
    
    lastMousePos = curMousePos;
    curMousePos = sf::Mouse::getPosition(window);
    if (curMousePos != lastMousePos)
    {
        float dx,dy;
        sf::Vector2f centerOfTurretWorld;
        sf::Vector2i mouseCursorScreen(curMousePos.x,curMousePos.y);
        sf::Vector2f mouseCursorWorld;

        //Find turrent angle based on vector from tank origin to mouse.
        centerOfTurretWorld = teamMan.teams[myTeam].players[mySlot].tank.position;
        mouseCursorWorld = window.convertCoords(mouseCursorScreen);
        dx = mouseCursorWorld.x - centerOfTurretWorld.x;
        dy = mouseCursorWorld.y - centerOfTurretWorld.y;
        //teamMan.teams[myTeam].players[mySlot].tank.turretAngle =  (180.0f/3.14156f)*atan2(dy,dx);
        float newTurretAngle =  (180.0f/3.14156f)*atan2(dy,dx);
        control = true;
        lastActionReg = lastActionReg | 0x000000010;
    }
    //update player 1 velocity based on updated throttle and body angle.
    /*teamMan.teams[my_team].players[0].tank.tankState.velocity.x =  teamMan.teams[my_team].players[0].tank.tankState.throttle * (float)cos(teamMan.teams[my_team].players[0].tank.tankState.bodyAngle / (180/3.14156));
    teamMan.teams[my_team].players[0].tank.tankState.velocity.y =  teamMan.teams[my_team].players[0].tank.tankState.throttle * (float)sin(teamMan.teams[my_team].players[0].tank.tankState.bodyAngle / (180/3.14156));
    */
    bool attacking = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        attacking = true;
        lastActionReg = lastActionReg | 0x000000020;
        std::cout << "Bang!" << std::endl;
    }

    if (lastActionReg != 0){
        Player & p = teamMan.getPlayerBySlot(myTeam, mySlot);
        p.state = PlayerState::SendingAction;
    }

    return 0;
}



