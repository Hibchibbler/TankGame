/**********************************************************************************
Tank Game
Comm.cpp

Copyright 2012
Daniel Ferguson, Eddie Stranberg
**********************************************************************************/

#include "Comm.h"
#include <iostream>

#define SELECTOR_TIMEOUT            0.001f
#define COMM_THREAD_LOOP_DELAY      40000 //40 ms




bool tg::Comm::StartClient(sf::Uint16 port, sf::IpAddress addr)
    
{//We are creating a connecting client
    this->address = addr;
    this->port = port;

    NotDone = true;
    AddConnection(tg::Connection());
    std::cout << "StartClient" << std::endl;
    CommLooperThread.launch();
    return true;
}

//TODO: StartServer operates on the Established connections container
bool tg::Comm::StartServer(sf::Uint16 port)  
{//We are creating a listener
    NotDone = true;
    Listener.listen(8280);
    ListeningSelector.add(Listener);
    std::cout << "StartServer" << std::endl;
    CommLooperThread.launch();
    return true;
}

void tg::Comm::Stop()
{
    NotDone = false;
    std::cout << "waiting for Looper Thread to stop" << std::endl;
    CommLooperThread.wait();

    Listener.close();
    std::vector<tg::Connection>::iterator i = Connecting.begin();
    for (;i != Connecting.end();i++){
        delete i->Socket;
        delete i->SendMutex;
        delete i->RecvMutex;
    }
    Connecting.clear();

    i = Established.begin();
    for (;i != Established.end();i++){
        i->Socket->disconnect();
        delete i->Socket;
        delete i->SendMutex;
        delete i->RecvMutex;
    }
    Established.clear();
}

 //Send operates on the Established connections container
void tg::Comm::Send(tg::CommEvent &gpacket)
{
    EstablishedMutex.lock();
    std::vector<tg::Connection>::iterator i = Established.begin();
    for (;i != Established.end();i++){
        if (gpacket.connectionId == -1 || gpacket.connectionId == i->connectionId){
            i->SendMutex->lock();
            i->SendQueue.push(gpacket.packet);
            i->SendMutex->unlock();
        }
    }
    EstablishedMutex.unlock();
}

//Receive operates on the Established connections container
bool tg::Comm::Receive(tg::CommEvent &gpacket)
{
    //first check for system messages
    SystemMutex.lock();
    while (!SystemPackets.empty()){
        gpacket.connectionId = -1;
        gpacket.packet = SystemPackets.front();
        SystemPackets.pop();
        SystemMutex.unlock();
        return true;
    }
    SystemMutex.unlock();


    //now check for data
    EstablishedMutex.lock();
    std::vector<tg::Connection>::iterator i = Established.begin();
    for (;i != Established.end();i++){
        i->RecvMutex->lock();
        if (!i->RecvQueue.empty()){
            gpacket.packet = i->RecvQueue.front();
            gpacket.connectionId = i->connectionId;
            i->RecvQueue.pop();
            i->RecvMutex->unlock();
            EstablishedMutex.unlock();
            return true;
        }
        i->RecvMutex->unlock();
    }
    EstablishedMutex.unlock();
    return false;
}

//AddConnection adds to the Connecting connections container
void tg::Comm::AddConnection(tg::Connection &client)
{
    ConnectingMutex.lock();
    Connecting.push_back(client);
    ConnectingMutex.unlock();
}


std::vector<sf::Uint32> tg::Comm::getConnectionIds()
{
    EstablishedMutex.lock();
    std::vector<sf::Uint32> cids;
    for (auto i = Established.begin();i != Established.end();i++){
        cids.push_back(i->connectionId);
    }
    EstablishedMutex.unlock();
    return cids;
}

void tg::Comm::CommLooper(Comm* comm)
{
    std::cout << "Looper" << std::endl;
    
    while (comm->NotDone){

        // Check for, and process, any new connection requests
        //TODO: spawn a thread for each willing listener;  the listener mutex and selector stuff is WIP.
        if (comm->ListeningSelector.wait(sf::microseconds(3))){
            tg::Connection newConnection;
            newConnection.Socket = new sf::TcpSocket();
            newConnection.SendMutex = new sf::Mutex();
            newConnection.RecvMutex = new sf::Mutex();
            
            sf::Socket::Status s= comm->Listener.accept(*newConnection.Socket);
            if (s == sf::Socket::Done){
                std::cout << "Client connected from " << newConnection.Socket->getRemoteAddress().toString() << std::endl;

                // Add the client to our internal list, and add the client to the selector
                newConnection.connectionId = comm->TotalConnectCount;
                newConnection.IsConnected = true;
                
                comm->EstablishedMutex.lock();
                comm->Established.push_back(newConnection);
                comm->EstablishedSelector.add(*newConnection.Socket);
                comm->EstablishedMutex.unlock();

                comm->SendSystem(CommEventType::Acceptance, comm->TotalConnectCount, std::string("accepted Connection Request"));
                comm->TotalConnectCount++;
            }else{
                //TODO: Handle This!!!!!!!!!
                std::cout << "Unable to accept" << std::endl;
            }
        }
    
        //Check for and process and new outgoing connections
        comm->ConnectingMutex.lock();
        {
            std::vector<tg::Connection>::iterator i;
            i = comm->Connecting.begin();
            while (i != comm->Connecting.end()){
                if (i->Socket == NULL){
                    i->Socket = new sf::TcpSocket();
                    i->SendMutex = new sf::Mutex();
                    i->RecvMutex = new sf::Mutex();
                }
                sf::Socket::Status s = i->Socket->connect(comm->address, comm->port);//"192.168.2.110"
                if (s == sf::Socket::Done){
                
                    i->connectionId = comm->TotalConnectCount;
                    i->IsConnected = true;
                    
                    comm->EstablishedSelector.add(*i->Socket);
                    comm->Established.push_back(*i);//this must run before the QueueSystemMessage
                    
                    comm->SendSystem(CommEventType::Acceptance,comm->TotalConnectCount, std::string("Connected"));
                    comm->TotalConnectCount++;
                    //The last thing we do.
                    i = comm->Connecting.erase(i);
                }else {
                    if(s == sf::Socket::Disconnected)
                        comm->SendSystem(CommEventType::Disconnect, comm->TotalConnectCount, std::string("Failed to Connect to Remote Host"));
                    else
                        comm->SendSystem(CommEventType::Error,  comm->TotalConnectCount, std::string("Error on connect"));
                    //we'd better remove the socket from the selector
                    delete i->Socket;
                    delete i->SendMutex;
                    delete i->RecvMutex;
                    i = comm->Connecting.erase(i);
                }

            }
            
        }comm->ConnectingMutex.unlock();
        

        comm->EstablishedMutex.lock();
        {
            //Retrieve new incoming data
            if ( comm->EstablishedSelector.wait(sf::milliseconds(3)) ) {
                //Some data is available
                std::vector<tg::Connection>::iterator i;
                i = comm->Established.begin();
                
                while(i != comm->Established.end()){
                    bool ok=true;
                    if (comm->EstablishedSelector.isReady(*i->Socket)){
                        //Client Ready to receive

                        tg::CommEvent RecvPacket;
                        RecvPacket.connectionId = i->connectionId;
                        sf::Socket::Status s;
                        s = i->Socket->receive(RecvPacket.packet);
                        if (s == sf::Socket::Done){
               
                            i->RecvMutex->lock();
                            i->RecvQueue.push(RecvPacket.packet);
                            i->RecvMutex->unlock();
                            ok=true;
                        }else{
                        
                            if(s == sf::Socket::Disconnected)
                                comm->SendSystem(CommEventType::Disconnect, RecvPacket.connectionId, std::string("Client disconnected"));
                            else
                                comm->SendSystem(CommEventType::Error, RecvPacket.connectionId, std::string("Error on receive"));
                            //we'd better remove the socket from the selector
                            comm->EstablishedSelector.remove(*i->Socket);
                            delete i->Socket;
                            delete i->SendMutex;
                            delete i->RecvMutex;
                            //i = comm->Established.erase(i);
                            ok=false;
                        }
                    }
                    if (ok){
                        i++;
                    }else{
                        i = comm->Established.erase(i);
                    }

                }
            }
            //Send any pending outgoing data
            

            std::vector<tg::Connection>::iterator connection =  comm->Established.begin();
            for (;connection != comm->Established.end();){
                bool ok = true;
                connection->SendMutex->lock();
                while(!connection->SendQueue.empty()){
                    sf::Packet packet = connection->SendQueue.front();
                    connection->SendQueue.pop();
                    sf::Socket::Status s = connection->Socket->send(packet);
                    if (s == sf::Socket::Done){
                        comm->SendSystem(CommEventType::Sent, connection->connectionId, std::string("Sent"));
                        ok = true;
                    }else{
                        if (s == sf::Socket::Disconnected)
                            comm->SendSystem(CommEventType::Disconnect, connection->connectionId, std::string("Client disconnected"));
                        else
                            comm->SendSystem(CommEventType::Error, connection->connectionId, std::string("Error on Send"));

                        //we'd better remove the socket from the selector
                        comm->EstablishedSelector.remove(*connection->Socket);
                        delete connection->Socket;
                        delete connection->SendMutex;
                        delete connection->RecvMutex;
                        ok = false;
                    }
                }
                connection->SendMutex->unlock();
                if (ok)
                    connection++;
                else{
                    connection = comm->Established.erase(connection);
                }
                
            }
        }comm->EstablishedMutex.unlock();
            
      

        //Sleep little baby
        sf::sleep(sf::milliseconds(0));
    }
}


void tg::Comm::SendSystem(CommEventType::CET cet, sf::Uint32 connectionId, std::string msg)
{
    sf::Packet packet;
    packet << cet << connectionId << msg;
    SystemMutex.lock();
    SystemPackets.push(packet);
    SystemMutex.unlock();
}


