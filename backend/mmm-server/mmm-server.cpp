#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <vector>

int main()
{
	bool running(true);

	//list of open connections
	std::vector<sf::SocketTCP*> currentClients;

	//create a socket listener
	sf::SocketTCP listener;
	//listener->SetBlocking(false);
	listener.Listen(53000);

	sf::SelectorTCP selector;
	selector.Add(listener);

	while(running)
	{
		unsigned int nbSockets = selector.Wait();

		for (unsigned int i = 0; i < nbSockets; ++i)
		{
			sf::SocketTCP socket = selector.GetSocketReady(i);

			if (socket == listener)
		    {
		        // If the listening socket is ready, it means that we can accept a new connection
		        sf::IPAddress address;
		        sf::SocketTCP client;
		        listener.Accept(client, &address);
		        std::cout << "Client connected ! (" << address << ")" << std::endl;

		        // Add it to the selector
		        selector.Add(client);
		    }
		    else
		    {
		        // Else, it is a client socket so we can read the data he sent
		        sf::Packet packet;
		        if (socket.Receive(packet) == sf::Socket::Done)
		        {
		            // Extract the message and display it
		            std::string message;
		            packet >> message;
		            std::cout << "A client says : \"" << message << "\"" << std::endl;
		        }
		        else
		        {
		            // Error : we'd better remove the socket from the selector
		            selector.Remove(socket);
		        }
		    }

		}

	}

    return 0;
}