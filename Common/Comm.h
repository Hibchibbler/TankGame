/**********************************************************************************
Tank Game
Comm.h

Copyright 2012
Daniel Ferguson, Eddie Stranberg
**********************************************************************************/

#ifndef Comm_h_
#define Comm_h_

#include <SFML/Network.hpp>
#include <queue>


namespace tg
{
    //class Connection contains information that is specific to a connection between two clients.
    // * Queue's for reads and writes.
    // * Mutexes to synchronize access to the queues.
    // * The socket used to read and write to the connection
    // * A unique identifier used to identify this connection. This identifier is only
    //   unique to an instance of Comm.
    class Connection
    {
    public:
        Connection(){
            IsConnected = false;
            Socket = NULL;
        }
        bool IsConnected;
        sf::TcpSocket* Socket;
        sf::Uint32 connectionId;
        std::queue<sf::Packet> RecvQueue;
        std::queue<sf::Packet> SendQueue;
        sf::Mutex* SendMutex;
        sf::Mutex* RecvMutex;
    private:

    };

    //CommEvent is a structure containing
    // information, pertaining to a particular event, that is passed to the user.
    struct CommEventType{
        enum CET{
            Acceptance,
            Established,
            Disconnect,
            Data,
            Sent,
            Error
        };
    };
    class CommEvent
    {
    public:
        
        CommEvent(){
            connectionId = -1;
        }
        sf::Packet packet;
        sf::Uint32 connectionId;
    private:
    };


    class Comm
    {
    public:
        Comm()
            : 
        CommLooperThread(tg::Comm::CommLooper,this){
            NotDone = true;
            TotalConnectCount=0;
        }
        ~Comm(){
        }
        void AddConnection(tg::Connection &client);
        bool StartClient(sf::Uint16 port, sf::IpAddress addr);
        bool StartServer(sf::Uint16 port);
        void Stop();
        void Send(tg::CommEvent &p);
        bool Receive(tg::CommEvent &p);

        std::vector<sf::Uint32> getConnectionIds();
    protected:
        sf::TcpListener Listener;

        static void CommLooper(Comm* comm);
        sf::Thread CommLooperThread;
        sf::SocketSelector ListeningSelector;
        sf::SocketSelector ConnectingSelector;
        sf::SocketSelector EstablishedSelector;
        
        sf::Mutex ListeningMutex;
        sf::Mutex ConnectingMutex;
        sf::Mutex EstablishedMutex;
        sf::Mutex SystemMutex;

        std::vector<tg::Connection> Listening;
        std::vector<tg::Connection> Connecting;
        std::vector<tg::Connection> Established;
        std::queue<sf::Packet> SystemPackets;

        bool NotDone;
        sf::Uint32 TotalConnectCount;

        //SendSystem enqueues a CommEvent into the incoming queues - where the instantiating scope will 
        // then be able to processes them along other subsystem events(sfml-window)
        void SendSystem(CommEventType::CET cet, sf::Uint32 connectionId, std::string msg);
    };
}

#endif



