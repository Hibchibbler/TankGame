#ifndef Player_h_
#define Player_h_

#include <SFML/Graphics.hpp>
#include "Tank.h"

namespace tg
{
    //This ratified enum is used by both server and client entities
    //to identify state. so, some states don't make sense from a client
    //perspective, and likewise for server.
    //None the less, the state is specific to a Player
    struct PlayerState{
    enum pstate{
            New,/***********/
            SendingWhoIs,
            SendingWhoIsAck,
            WaitingForWhoIsAck,
            SendingId,
            SendingIdAck,
            SendingIdNack,
            WaitingForIdAck,
            SendingStateOfPlayer,
            WaitingForStateOfPlayer,
            SendingStateOfUnion,
            WaitingForStateOfUnion,
            SendingReady,
            WaitingForReady,
            Ready,/***********/
            WaitingForMap,
            WaitingForStart,
            Running,/***********/
            Paused/***********/
        };
    };


    class Player
    {
    public:
        
        
        Player(){
            playerName = "-empty-";
            connectionId = -1;
            team = -1;
            state = PlayerState::New;
            health = 100;
            hasHost = false;
            slotNum = -1;
        }
        Tank tank;
        
        std::string playerName;
        sf::Uint32 connectionId;
        sf::Uint32 hasHost;//yes, like, in the alien sense.
        sf::Uint32 slotNum;
        sf::Uint32 team;
        sf::Uint32 health;
        sf::Uint32 state;
        
    };
};

#endif

