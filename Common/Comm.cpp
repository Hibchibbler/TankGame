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
    AddConnection(std::shared_ptr<tg::Connection>(new tg::Connection()));
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
    Connecting.clear();
    Established.clear();
}

 //Send operates on the Established connections container
void tg::Comm::Send(tg::CommEvent &gpacket)
{
    //EstablishedMutex.lock();
    std::vector<std::shared_ptr<tg::Connection> >::iterator i = Established.begin();
    for (;i != Established.end();i++){
        if (!(*i)->IsConnected)
            continue;
        if (gpacket.connectionId == -1 || gpacket.connectionId == (*i)->connectionId){
            (*i)->SendMutex.lock();
            (*i)->SendQueue.push(gpacket.packet);
            (*i)->SendMutex.unlock();
        }
    }
    //EstablishedMutex.unlock();
}

//Receive operates on the Established connections container
bool tg::Comm::Receive(tg::CommEvent &gpacket)
{
    //first check for system messages
    SystemMutex.lock();
    if(!SystemPackets.empty()){
        gpacket.connectionId = -1;
        gpacket.packet = SystemPackets.front();
        SystemPackets.pop();
        SystemMutex.unlock();
        return true;
    }
    SystemMutex.unlock();


    //now check for data
    //EstablishedMutex.lock();
    std::vector<std::shared_ptr<tg::Connection> >::iterator i = Established.begin();
    for (;i != Established.end();i++){
        if ((*i)->IsConnected && !(*i)->error)
        {
            (*i)->RecvMutex.lock();
            if (!(*i)->RecvQueue.empty()){
                gpacket.packet = (*i)->RecvQueue.front();
                gpacket.connectionId = (*i)->connectionId;
                (*i)->RecvQueue.pop();
                (*i)->RecvMutex.unlock();
                //EstablishedMutex.unlock();
                return true;
            }
        }
        (*i)->RecvMutex.unlock();
    }
    //EstablishedMutex.unlock();
    return false;
}

//AddConnection adds to the Connecting connections container
void tg::Comm::AddConnection(std::shared_ptr<tg::Connection> client)
{
    ConnectingMutex.lock();
    Connecting.push_back(client);
    ConnectingMutex.unlock();
}

void tg::Comm::CommLooper(Comm* comm)
{
    std::cout << "Looper" << std::endl;
    
    while (comm->NotDone){
        std::vector<std::shared_ptr<tg::Connection> >::iterator i;
        // Check for, and process, any new connection requests
        //TODO: spawn a thread for each willing listener; 
        //      the listener mutex and selector stuff is WIP.
        //      
        if (comm->ListeningSelector.wait(sf::microseconds(1))){

            int newConnIndex = 0;
            for (newConnIndex = 0 ; newConnIndex < 15;newConnIndex++)
            {
                if (!comm->Established[newConnIndex]->IsConnected)
                    break;
            }
            
            sf::Socket::Status s= comm->Listener.accept(comm->Established[newConnIndex]->Socket);
            if (s == sf::Socket::Done){
                std::cout << "Client connected from " << comm->Established[newConnIndex]->Socket.getRemoteAddress().toString() << std::endl;

                // Add the client to our internal list, and add the client to the selector
                comm->Established[newConnIndex]->connectionId = comm->TotalConnectCount;
                comm->Established[newConnIndex]->IsConnected = true;
                comm->EstablishedSelector.add(comm->Established[newConnIndex]->Socket);
                comm->SendSystem(CommEventType::Acceptance, comm->TotalConnectCount, std::string("accepted Connection Request"));
                comm->TotalConnectCount++;
            }else{
                //TODO: Handle This!!!!!!!!!
                std::cout << "Unable to accept" << std::endl;
            }
        }
    
        //Check for and process and new outgoing connections
        comm->ConnectingMutex.lock();
        i = comm->Connecting.begin();
        while (i != comm->Connecting.end()){
            sf::Socket::Status s = (*i)->Socket.connect(comm->address, comm->port);//"192.168.2.110"
            if (s == sf::Socket::Done){
                
                (*i)->connectionId = comm->TotalConnectCount;
                (*i)->IsConnected = true;
                    

                for (int y = 0 ; y < 15;y++)
                {
                    if (!comm->Established[y]->IsConnected)
                    {
                        comm->Established[y] = (*i);
                        break;
                    }
                }
                comm->EstablishedSelector.add((*i)->Socket);
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
                i = comm->Connecting.erase(i);
            }
        }
        comm->ConnectingMutex.unlock();


        //Retrieve new incoming data
        if ( comm->EstablishedSelector.wait(sf::microseconds(1)) ) {
            //Some data is available
            i = comm->Established.begin();
            for (;i != comm->Established.end();i++){
                if (!(*i)->IsConnected || (*i)->error)
                    continue;
                if (comm->EstablishedSelector.isReady((*i)->Socket)){
                    //Client Ready to receive

                    tg::CommEvent RecvPacket;
                    RecvPacket.connectionId = (*i)->connectionId;
                    sf::Socket::Status s;
                    s = (*i)->Socket.receive(RecvPacket.packet);
                    if (s == sf::Socket::Done){
               
                        (*i)->RecvMutex.lock();
                        (*i)->RecvQueue.push(RecvPacket.packet);
                        (*i)->RecvMutex.unlock();

                    }else{
                        
                        if(s == sf::Socket::Disconnected)
                            comm->SendSystem(CommEventType::Disconnect, RecvPacket.connectionId, std::string("Client disconnected"));
                        else
                            comm->SendSystem(CommEventType::Error, RecvPacket.connectionId, std::string("Error on receive"));
                        (*i)->error = true;
                        //we'd better remove the socket from the selector
                        comm->EstablishedSelector.remove((*i)->Socket);
                    }
                }
            }
        }

        //Send any pending outgoing data
        i =  comm->Established.begin();
        for (;i != comm->Established.end();i++){
            if (!(*i)->IsConnected || (*i)->error)
                continue;
            (*i)->SendMutex.lock();
            while(!(*i)->SendQueue.empty()){
                //std::cout << (*i)->SendQueue.size() << std::endl;
                sf::Packet packet = (*i)->SendQueue.front();
                (*i)->SendQueue.pop();
                sf::Socket::Status s = (*i)->Socket.send(packet);
                if (s == sf::Socket::Done){
                    //comm->SendSystem(CommEventType::Sent, (*i)->connectionId, std::string("Sent"));
                }else{
                    if (s == sf::Socket::Disconnected)
                        comm->SendSystem(CommEventType::Disconnect, (*i)->connectionId, std::string("Client disconnected"));
                    else
                        comm->SendSystem(CommEventType::Error, (*i)->connectionId, std::string("Error on Send"));
                    (*i)->error = true;
                    //we'd better remove the socket from the selector
                    comm->EstablishedSelector.remove((*i)->Socket);
                    break;
                }
            }
            (*i)->SendMutex.unlock();
        }

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


