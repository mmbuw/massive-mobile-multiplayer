#include "PlayerConnection.hpp"

PlayerConnection::PlayerConnection(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket) : 
	id_(id), ip_(ip), socket_(socket) {}