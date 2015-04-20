#include "ConnectionDatabase.hpp"

ConnectionDatabase::ConnectionDatabase() {}

void ConnectionDatabase::put_elements(sf::SocketTCP const& socket, PlayerConnection* player_connection)
{
	socketPlayerConnections_.insert(std::make_pair(socket, player_connection));
	ipPlayerConnections_.insert(std::make_pair(player_connection->ip_.ToInteger(), player_connection));
}

bool ConnectionDatabase::is_present(sf::IPAddress const& ip) const
{
	return ipPlayerConnections_.find(ip.ToInteger()) != ipPlayerConnections_.end();
}

bool ConnectionDatabase::remove_element(sf::SocketTCP const& socket)
{
	auto it = socketPlayerConnections_.find(socket);

	if (it != socketPlayerConnections_.end())
	{
		int ip(it->second->ip_.ToInteger());
		socketPlayerConnections_.erase(it);
		ipPlayerConnections_.erase(ipPlayerConnections_.find(ip));
		return true;
	}

	return false;
}

PlayerConnection* ConnectionDatabase::get_player_connection(sf::SocketTCP const& socket)
{
	auto it = socketPlayerConnections_.find(socket);

	if (it != socketPlayerConnections_.end())
	{
		return it->second;
	}

	return nullptr;
}

PlayerConnection* ConnectionDatabase::get_player_connection(sf::IPAddress const& ip)
{
	auto it = ipPlayerConnections_.find(ip.ToInteger());

	if (it != ipPlayerConnections_.end())
	{
		return it->second;
	}

	return nullptr;
}

std::map<int, PlayerConnection*>::iterator const ConnectionDatabase::get_ip_begin_iterator()
{
	return ipPlayerConnections_.begin();
}

std::map<int, PlayerConnection*>::iterator const ConnectionDatabase::get_ip_end_iterator()
{
	return ipPlayerConnections_.end();
}