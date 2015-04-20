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

std::map<sf::SocketTCP, PlayerConnection*>::iterator ConnectionDatabase::remove_element_it(sf::SocketTCP const& socket)
{
	auto it = socketPlayerConnections_.find(socket);

	if (it != socketPlayerConnections_.end())
	{
		int ip(it->second->ip_.ToInteger());
		ipPlayerConnections_.erase(ipPlayerConnections_.find(ip));
		return socketPlayerConnections_.erase(it);
	}

	return socketPlayerConnections_.end();
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

void ConnectionDatabase::clean_timeout_connections(sf::Selector<sf::SocketTCP>& selector)
{
	for(std::map<sf::SocketTCP, PlayerConnection*>::iterator it = socketPlayerConnections_.begin(); it != socketPlayerConnections_.end();)
	{
		if (it->second->checkAlive() == false)
		{
			PlayerConnection* playerConnection = it->second;
			std::cout << "[Timeout] " << playerConnection->ip_ << " (Client ID " << playerConnection->id_ << ")" << std::endl;
			
			selector.Remove(it->first);
			it = remove_element_it(it->first);
			delete playerConnection;
		}
		else
		{
			++it;
		}
	}
}