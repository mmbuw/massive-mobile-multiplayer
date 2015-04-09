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

	bool running(true);

	while (running)
	{
		std::string message;
		std::getline(std::cin, message);

		sf::Packet toSend;
		toSend << message;

		if (client.Send(toSend) != sf::Socket::Done)
		{
			std::cout << "Error: Sending message failed." << std::endl;
			return 1;
		}

	}

    return 0;
}