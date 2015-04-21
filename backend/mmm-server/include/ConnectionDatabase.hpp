#ifndef CONNECTION_DATABASE_HPP
#define CONNECTION_DATABASE_HPP

#include <SFML/Network.hpp>
#include <map>

#include "PlayerConnection.hpp"

class ConnectionDatabase
{
	public:
		ConnectionDatabase();
		void putElements(sf::SocketTCP const& socket, PlayerConnection* player_connection);
		bool removeElement(sf::SocketTCP const& socket);
		PlayerConnection* getPlayerConnection(sf::SocketTCP const& socket);
		PlayerConnection* getPlayerConnection(sf::IPAddress const& ip);
		void cleanTimeoutConnections(sf::Selector<sf::SocketTCP>& selector);

	private:
		std::map<sf::SocketTCP, PlayerConnection*>::iterator removeElementIt(sf::SocketTCP const& socket);
		
		std::map<sf::SocketTCP, PlayerConnection*> socketPlayerConnections_;
		std::map<int, PlayerConnection*> ipPlayerConnections_;
};


#endif //CONNECTION_DATABASE_HPP