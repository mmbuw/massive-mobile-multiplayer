#ifndef PLAYER_SOCKET_HPP
#define PLAYER_SOCKET_HPP

#include <SFML/Network.hpp>

class PlayerSocket : public sf::SocketTCP
{
	public:
		PlayerSocket(int id);

		int id_;
		sf::IPAddress ip_;
};


#endif //PLAYER_SOCKET_HPP