#ifndef Team_h_
#define Team_h_

#include "Player.h"
#include <vector>


namespace tg
{

    class Team
    {
    public:
        Team(){
            players.reserve(5);
        }
        typedef std::vector<Player>::iterator PlayerIterator;
        std::vector<Player> players;
    };
};


#endif