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
        case PlayerState::SendingActionControl:
            Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,false);
            p.state = PlayerState::Running;
            break;
        case PlayerState::SendingActionAttack:
            Messages::sendAction(comm,teamMan, myCID,myTeam,mySlot,true);
            p.state = PlayerState::Running;
            break;
        case PlayerState::Running:
            break;
    }
    return getSummary(0);
}
sf::Uint32 StageRun::doLocalInput(TeamManager & teamMan)
{
    sf::Uint32 scrWidth = getInput(0);
    sf::Uint32 scrHeight = getInput(1);
    sf::Uint32 myCID = getInput(2);
    sf::Uint32 myTeam = getInput(3);
    sf::Uint32 mySlot = getInput(4);

    bool control = false;
    //We poll keyboard instead of relying on bios keyboard rate
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
        teamMan.teams[myTeam].players[mySlot].tank.throttle += 1;
        if (teamMan.teams[myTeam].players[mySlot].tank.throttle > 25)
            teamMan.teams[myTeam].players[mySlot].tank.throttle = 25;  
        control = true;
    }
    

    //max speed lower in reverse
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        teamMan.teams[myTeam].players[mySlot].tank.throttle -= 1;
        if (teamMan.teams[myTeam].players[mySlot].tank.throttle < -10)
            teamMan.teams[myTeam].players[mySlot].tank.throttle = -10;  
        control = true;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        //Rotating left
        teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = teamMan.teams[myTeam].players[mySlot].tank.bodyAngle -  5;
        if (teamMan.teams[myTeam].players[mySlot].tank.bodyAngle >= 360.0f || teamMan.teams[myTeam].players[mySlot].tank.bodyAngle <= -360.0f ){
            teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = 0;
        }
        control = true;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        //Rotating right
        teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = teamMan.teams[myTeam].players[mySlot].tank.bodyAngle +  5;
        if (teamMan.teams[myTeam].players[mySlot].tank.bodyAngle >= 360.0f || teamMan.teams[myTeam].players[mySlot].tank.bodyAngle <= -360.0f ){
            teamMan.teams[myTeam].players[mySlot].tank.bodyAngle = 0;
        }
    }

    //update player 1 velocity based on updated throttle and body angle.
    /*teamMan.teams[my_team].players[0].tank.tankState.velocity.x =  teamMan.teams[my_team].players[0].tank.tankState.throttle * (float)cos(teamMan.teams[my_team].players[0].tank.tankState.bodyAngle / (180/3.14156));
    teamMan.teams[my_team].players[0].tank.tankState.velocity.y =  teamMan.teams[my_team].players[0].tank.tankState.throttle * (float)sin(teamMan.teams[my_team].players[0].tank.tankState.bodyAngle / (180/3.14156));
    */
    bool attacking = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        attacking = true;

        std::cout << "Bang!" << std::endl;
    }

    if (control){
        //Messages::sendAction(comm, teamMan, myCID, myTeam, mySlot, attacking);
        Player & p = teamMan.getPlayerBySlot(myTeam, mySlot);
        if (attacking)
            p.state = PlayerState::SendingActionAttack;
        else
            p.state = PlayerState::SendingActionControl;

    }
    return 0;
}



