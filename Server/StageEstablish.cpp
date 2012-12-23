#include "StageEstablish.h"
#include "Common\Comm.h"
#include "Common\Messages.h"
#include "Common\TeamManager.h"
#include "Common\ArenaManager.h"
#include "GameServer.h"

#include <iostream>
#include "Common\LogFile.h"

using namespace tg;

StageEstablish::StageEstablish()
    : GameStage()
{

}

sf::Uint32 StageEstablish::doRemoteEvent(Game & g,
                                         CommEvent & cevent,
                                         sf::Uint32 connId,
                                         sf::Uint32 msgId)
{

    //LogFile::get()->log(0,0,"StageEstablish::doRemoteEvents");
    Player &player = g.teamMan.getPlayer(connId);

    switch (msgId){
        case MsgId::Ready:
        {
            std::cout << "Got Ready" << std::endl;
            player.state = PlayerState::Ready;
            //p = teamMan.getPlayer(cid);
            /*tg::Team::PlayerIterator & pi1 = teamMan.teams[1].players.begin();
            tg::Team::PlayerIterator & pi2 = teamMan.teams[2].players.begin();*/

            break;
        }
        case MsgId::WhoIs:
            std::cout << "Got WhoIs" << std::endl;
            //Messages::sendWhoIsAck(comm, teamMan, player.connectionId);
            player.state = PlayerState::SendingWhoIsAck;
            break;
        case MsgId::Id:{
                        
            
            //Received Id msg
            std::string name;
            sf::Uint32 team;
            cevent.packet >> name;
            cevent.packet >> team;
            std::cout << "Got Id: " << name << ", " << team << std::endl;
            
            if (g.teamMan.isIdValid(name, team))
            {
                //the specified name and team are ok
                //remove player form limbo
                g.teamMan.removePlayer(player.connectionId);
                            
                //add player to requested team
               /* static float x = 10;
                sf::Vector2f pos(x,20);
                x+=20;*/

                player.tank.position = g.arenaMan.getStartPosition(team);
                player.state = PlayerState::SendingIdAck;
                std::cout << "Adding " << name << " to team " << team << std::endl;
                sf::Uint32 slot = g.teamMan.addPlayer(player, name, team);
                
                //tell player they are established
                //Messages::sendIdAck(comm, teamMan, player.connectionId, slot);
                //player.state = PlayerState::SendingIdAck;
            }else{
                //Messages::sendIdNack(comm, teamMan, player.connectionId);
                player.state = PlayerState::SendingIdNack;
            }
            
            break;
        }case MsgId::StateOfPlayer:{
            std::cout << "Got Action" << std::endl;
            //These are from players when they change the state of the game via an input mechanism
            //After crunching this event, send a Result packet to each player.
           

            break;
        }
    }
    return 0;
}


sf::Uint32 StageEstablish::doLoop(Game & g)
{
    //LogFile::get()->log(0,0,"StageEstablish::doLoop");
    for (int y = 0; y < 3 ; y++)
    {
        tg::Team::PlayerIterator & pi = g.teamMan.getTeam(y).begin();
        for (;pi != g.teamMan.getTeam(y).end();pi++){
        
            if (pi->hasHost == false)
                continue;
    
            switch (pi->state){
            case PlayerState::New:
                //this guy is new....
                //we're expecting to receive a WhoIs and then some Id
                //at some point soon.
                break;
            case PlayerState::SendingWhoIsAck:
                Messages::sendWhoIsAck(g.server, g.teamMan, pi->connectionId);
                pi->state = PlayerState::New;
                break;
            case PlayerState::SendingIdAck:
                Messages::sendIdAck(g.server, g.teamMan, pi->connectionId, pi->slotNum);
                Messages::sendWhoIsAck(g.server,g.teamMan,-1);
                pi->state = PlayerState::WaitingForReady;
                break;
            case PlayerState::SendingIdNack:
                Messages::sendIdNack(g.server, g.teamMan, pi->connectionId);
                pi->state = PlayerState::New;
                break;

            }
        }
    }
    return getSummary(0);
}

sf::Uint32 StageEstablish::doLocalInput(sf::RenderWindow & window, Game & g)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            std::cout << "!";
            bool someNotReady = false;
            for (int y = 1; y < 3 ; y++)
            {
                tg::Team::PlayerIterator & pi = g.teamMan.getTeam(y).begin();
                for (;pi != g.teamMan.getTeam(y).end();pi++){
                    if  (pi->hasHost == 1){
                        if (pi->state != PlayerState::Ready){
                            std::cout
                                << "nostart";
                            someNotReady = true;
                        }
                    }
                }
            }
            if (!someNotReady){
                setSummary(1,0);
                std::cout << "@";
            }
            //serverState = Running;
        }
    }

    return 0;
}











