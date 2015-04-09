#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>

int main()
{
	//create a socket listener
	sf::SocketTCP listener;

	if (!listener.Listen(53000))
	{
		std::cout << "Error: Listener could not be initialized." << std::endl;
		return 1;
	}

	//wait for connections
	sf::IPAddress clientAddress;
	sf::SocketTCP client;

	if (listener.Accept(client, &clientAddress) != sf::Socket::Done)
	{
		std::cout << "Error: Accepting connection to client failed." << std::endl;
	}

	std::cout << "Connection to a client established." << std::endl;

    return 0;
}