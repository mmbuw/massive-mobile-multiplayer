#ifndef PLAYER_CONNECTION_HPP
#define PLAYER_CONNECTION_HPP

#include <SFML/Network.hpp>

#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <thread>
#include <chrono>


#define CONNECTION_TIMEOUT 30 //seconds

class PlayerConnection
{
	public:
		PlayerConnection(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket);
		~PlayerConnection();
		void injectKeyEvent(int eventCode) const;
		void injectRelEvent(int xCoord, int yCoord) const;
		void unregisterEventDevice();
		bool checkAlive() const;
		void sendViaSocket(std::string const& message);

		int id_;
		sf::IPAddress ip_;
		sf::SocketTCP socket_;
		int uinputHandle_;
		uinput_user_dev eventDevice_;

		mutable std::chrono::time_point<std::chrono::system_clock> lastInputTime_;
};


#endif //PLAYER_CONNECTION_HPP