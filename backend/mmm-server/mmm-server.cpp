#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>

#include "PlayerSocket.hpp"

int main()
{
	bool running(true);
	int globalConnectionCounter(0);
	std::vector<PlayerSocket> currentConnections;

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
		        std::cout << "Client connected ! (" << address << ")" << std::endl;

		        //add the new socket to the selector
		        ++globalConnectionCounter;
		        PlayerSocket playerSocket(globalConnectionCounter, address, client);
		        currentConnections.push_back(playerSocket);
		        selector.Add(client);
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

		            PlayerSocket playerSocket;

		            for (std::vector<PlayerSocket>::iterator it = currentConnections.begin(); it < currentConnections.end(); ++it)
		            {
		            	if ( it->socket_ == socket )
		            	{
		            		playerSocket = (*it);
		            		break;
		            	}
		            }

		            std::cout << "[Client " << playerSocket.id_ << "]: \"" << message << "\"" << std::endl;
		        }
		        else
		        {
		            //the connection is lost
		            std::cout << "Remove a client" << std::endl;
		            selector.Remove(socket);
		        }
		    }

		}

	}

    return 0;
}