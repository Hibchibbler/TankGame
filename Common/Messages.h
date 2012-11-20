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
        StateOfUnion,
        StateOfPlayer,
        Map
    };};


    struct PlayerAction{
        enum PA{
            None,
            Throttle,
            Turn,
            Turret,
            Attack
        };
    };


    class MsgData
    {
    public:
        int cid;
        MsgId dataType;
    };

    class MsgDataReady : public MsgData
    {
    public:
   
    };

    class MsgDataAction : public MsgData
    {
    public:
   
        sf::Uint32 controlOrAttack;
    };

    class MsgDataWhoIs : public MsgData
    {
    public:
   
    };
    class MsgDataWhoIsAck : public MsgData
    {
    public:
       std::vector<std::string> team1;
       std::vector<std::string> team2;
    };

    class MsgDataId : public MsgData
    {
    public:
       std::string name;
       sf::Uint32 team;
    };

    class MSG_DATA_IDACK : public MsgData
    {
    public:
       sf::Uint32 slot;
    };

    class MsgDataIdNack : public MsgData
    {
    public:
    };

    class Comm;
    class TeamManager;
    class Player;

    class Messages
    {
    public:
        static int sendReady(Comm & comm, TeamManager & teamMan, int cid);
        //static MsgDataReady extractReady(sf::Packet & packet);

        static int sendStateOfPlayer(Comm & comm, TeamManager & teamMan, int cid, int team, int slot, Player & player, sf::Uint32 attacking);

        static int sendWhoIs(Comm & comm, TeamManager & teamMan, int cid);
        static int sendId(Comm & comm, TeamManager & teamMan, int cid, std::string name, int team);

        static int sendWhoIsAck(Comm & comm, TeamManager & teamMan, int cid);

        static int sendIdAck(Comm & comm, TeamManager & teamMan, int cid, sf::Uint32 slot);
        static int sendIdNack(Comm & comm, TeamManager & teamMan, int cid);

        static int sendStateOfUnion(Comm & comm, TeamManager & teamMan);
        static int sendStart(Comm & comm, TeamManager & teamMan, int cid);

        static int sendMap(Comm & comm, std::string name, int cid);
    };



};


#endif





