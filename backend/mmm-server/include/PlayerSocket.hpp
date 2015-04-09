#ifndef PLAYER_SOCKET_HPP
#define PLAYER_SOCKET_HPP

#include <SFML/Network.hpp>

class PlayerSocket
{
	public:
		PlayerSocket();
		PlayerSocket(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket);

		int id_;
		sf::IPAddress ip_;
		sf::SocketTCP socket_;
};


#endif //PLAYER_SOCKET_HPP