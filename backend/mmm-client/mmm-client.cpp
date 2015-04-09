#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>

int main()
{
	sf::SocketTCP client;

	if (client.Connect(53000, "localhost") != sf::Socket::Done)
	{
		std::cout << "Error: Building connection to server failed." << std::endl;
		return 1;
	}

    return 0;
}