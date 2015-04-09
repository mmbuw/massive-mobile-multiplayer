#ifndef PLAYER_CONNECTION_HPP
#define PLAYER_CONNECTION_HPP

#include <SFML/Network.hpp>

class PlayerConnection
{
	public:
		PlayerConnection(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket);

		int id_;
		sf::IPAddress ip_;
		sf::SocketTCP socket_;
};


#endif //PLAYER_CONNECTION_HPP