#ifndef CONNECTION_DATABASE_HPP
#define CONNECTION_DATABASE_HPP

#include <SFML/Network.hpp>
#include <map>
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
		std::map<int, PlayerConnection*>::iterator const get_ip_begin_iterator();
		std::map<int, PlayerConnection*>::iterator const get_ip_end_iterator();

	private:
		std::map<sf::SocketTCP, PlayerConnection*> socketPlayerConnections_;
		std::map<int, PlayerConnection*> ipPlayerConnections_;
};


#endif //CONNECTION_DATABASE_HPP