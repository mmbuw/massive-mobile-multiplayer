#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <map>

#include "PlayerConnection.hpp"

int main()
{
	bool running(true);
	int globalConnectionCounter(0);
	std::map<sf::SocketTCP, PlayerConnection*> socketPlayerConnetions;

	//create a socket listener
	sf::SocketTCP listener;
	listener.Listen(53000);

	sf::SelectorTCP selector;
	selector.Add(listener);

	while(running)
	{
		//wait until at least one socket has news
		unsigned int nbSockets = selector.Wait();

		//iterate over all sockets to process
		for (unsigned int i = 0; i < nbSockets; ++i)
		{
			sf::SocketTCP socket = selector.GetSocketReady(i);

			//if the listening socket is ready, we can accept a new connection
			if (socket == listener)
		    {
		        sf::IPAddress address;
		        sf::SocketTCP client;
		        listener.Accept(client, &address);

		        //add the new socket to the selector
		        ++globalConnectionCounter;
		        PlayerConnection* playerConnection = new PlayerConnection(globalConnectionCounter, address, client);
		        socketPlayerConnetions.insert(std::make_pair(client, playerConnection));
		        selector.Add(client);

		        std::cout << "Client " << globalConnectionCounter << " (" << address << ") connected." << std::endl;
		    }

		    //else it is a client socket, so we read the message which was sent
		    else
		    {
		        sf::Packet packet;

		        if (socket.Receive(packet) == sf::Socket::Done)
		        {
		         	//extract the message and print it
		            std::string message;
		            packet >> message;

		            PlayerConnection* playerConnection = socketPlayerConnetions.find(socket)->second;

		            std::cout << "[Client " << playerConnection->id_ << "]: \"" << message << "\"" << std::endl;
		            playerConnection->injectEvent(EV_KEY, KEY_D);
		        }
		        else
		        {
		            //the connection is lost
		            std::map<sf::SocketTCP, PlayerConnection*>::iterator mapIteratorToDelete = socketPlayerConnetions.find(socket);
		            std::cout << "Client " << mapIteratorToDelete->second->id_ << " disconnected." << std::endl;

		            delete mapIteratorToDelete->second;
		            socketPlayerConnetions.erase(mapIteratorToDelete);
		            selector.Remove(socket);
		        }
		    }

		}

	}

    return 0;
}