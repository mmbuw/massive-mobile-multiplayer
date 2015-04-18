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
#include <atomic>

class PlayerConnection
{
	public:
		PlayerConnection(int id, sf::IPAddress const& ip, sf::SocketTCP const& socket);
		~PlayerConnection();
		bool isResponding() const;
		void injectKeyEvent(int eventCode) const;
		void injectRelEvent(int xCoord, int yCoord) const;
		void unregisterEventDevice();
		void checkPingThreadTask();

		int id_;
		sf::IPAddress ip_;
		sf::SocketTCP socket_;
		int uinputHandle_;
		uinput_user_dev eventDevice_;

		std::atomic<bool> checkAliveRunning_;
		std::atomic<bool> clientResponding_;
		std::thread checkAliveThread_;
};


#endif //PLAYER_CONNECTION_HPP