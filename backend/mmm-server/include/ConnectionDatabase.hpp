#ifndef CONNECTION_DATABASE_HPP
#define CONNECTION_DATABASE_HPP

#include <SFML/Network.hpp>
#include "PlayerConnection.hpp"

class ConnectionDatabase
{
	public:
		ConnectionDatabase();
		void put_elements(sf::SocketTCP const& socket, PlayerConnection* player_connection);
		bool is_present(sf::IPAddress const& ip) const;
		bool remove_element(sf::SocketTCP const& socket);
		PlayerConnection* get_player_connection(sf::SocketTCP const& socket);
		PlayerConnection* get_player_connection(sf::IPAddress const& ip);

	private:
		std::map<sf::SocketTCP, PlayerConnection*> socketPlayerConnections;
		std::map<int, PlayerConnection*> ipPlayerConnections;
};


#endif //CONNECTION_DATABASE_HPP