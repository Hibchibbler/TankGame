#ifndef Messages_h_
#define Messages_h_

#include <vector>
#include <string>
#include <SFML/Config.hpp>

namespace tg
{
    struct MsgId {enum mid{
        WhoIs,
        WhoIsAck,
        Id,
        IdAck,
        IdNack,
        Ready,
        Start,
        Action,
        State
    };};

    struct PlayerAction{
        enum PA{
            None,
            ThrottleUp,
            ThrottleDown,
            BodyRight,
            BodyLeft,
            TurretMove,
            Attack
        };
    };


    class MSG_DATA
    {
    public:
        int cid;
        MsgId dataType;
    };

    class MSG_DATA_READY : public MSG_DATA
    {
    public:
   
    };

    class MSG_DATA_ACTION : public MSG_DATA
    {
    public:
   
        sf::Uint32 controlOrAttack;
    };

    class MSG_DATA_WHOIS : public MSG_DATA
    {
    public:
   
    };
    class MSG_DATA_WHOISACK : public MSG_DATA
    {
    public:
       std::vector<std::string> team1;
       std::vector<std::string> team2;
    };

    class MSG_DATA_ID : public MSG_DATA
    {
    public:
       std::string name;
       sf::Uint32 team;
    };

    class MSG_DATA_IDACK : public MSG_DATA
    {
    public:
       sf::Uint32 slot;
    };

    class MSG_DATA_IDNACK : public MSG_DATA
    {
    public:
    };

    class Comm;
    class TeamManager;
    

    class Messages
    {
    public:
        static int sendReady(Comm & comm, TeamManager & teamMan, int cid);


        static int sendAction(Comm & comm, TeamManager & teamMan, int cid, int team, int slot, PlayerAction::PA playerAction);

        static int sendWhoIs(Comm & comm, TeamManager & teamMan, int cid);
        static int sendId(Comm & comm, TeamManager & teamMan, int cid, std::string name, int team);

        static int sendWhoIsAck(Comm & comm, TeamManager & teamMan, int cid);

        static int sendIdAck(Comm & comm, TeamManager & teamMan, int cid, sf::Uint32 slot);
        static int sendIdNack(Comm & comm, TeamManager & teamMan, int cid);

        static int sendState(Comm & comm, TeamManager & teamMan);
        static int sendStart(Comm & comm, TeamManager & teamMan, int cid);
    };



};


#endif





