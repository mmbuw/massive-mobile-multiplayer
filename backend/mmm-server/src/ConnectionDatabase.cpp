#include "ConnectionDatabase.hpp"

ConnectionDatabase::ConnectionDatabase() {}

void ConnectionDatabase::put_elements(sf::SocketTCP const& socket, PlayerConnection* player_connection)
{
	socketPlayerConnections.insert(std::make_pair(socket, player_connection));
	ipPlayerConnections.insert(std::make_pair(player_connection->ip_.ToInteger(), player_connection));
}

bool ConnectionDatabase::is_present(sf::IPAddress const& ip) const
{
	return ipPlayerConnections.find(ip.ToInteger()) != ipPlayerConnections.end();
}

bool ConnectionDatabase::remove_element(sf::SocketTCP const& socket)
{
	auto it = socketPlayerConnections.find(socket);

	if (it != socketPlayerConnections.end())
	{
		int ip(it->second->ip_.ToInteger());
		socketPlayerConnections.erase(it);
		ipPlayerConnections.erase(ipPlayerConnections.find(ip));
		return true;
	}

	return false;
}

PlayerConnection* ConnectionDatabase::get_player_connection(sf::SocketTCP const& socket)
{
	auto it = socketPlayerConnections.find(socket);

	if (it != socketPlayerConnections.end())
	{
		return it->second;
	}

	return nullptr;
}

PlayerConnection* ConnectionDatabase::get_player_connection(sf::IPAddress const& ip)
{
	auto it = ipPlayerConnections.find(ip.ToInteger());

	if (it != ipPlayerConnections.end())
	{
		return it->second;
	}

	return nullptr;
}