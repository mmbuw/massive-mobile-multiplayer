#include "PlayerSocket.hpp"

PlayerSocket::PlayerSocket() :
	id_(-1), ip_(-1), socket_(sf::SocketTCP()) {}

PlayerSocket::PlayerSocket(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket) : 
	id_(id), ip_(ip), socket_(socket) {}