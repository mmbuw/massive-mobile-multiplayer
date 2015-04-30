#ifndef PLAYER_CONNECTION_HPP
#define PLAYER_CONNECTION_HPP

//SFML
#include <SFML/Network.hpp>

//UInput dependencies
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>

//C++
#include <iostream>
#include <cstring>
#include <sstream>

class PlayerConnection
{
	public: 
		PlayerConnection(sf::TcpSocket* socket);
		~PlayerConnection();

		void injectKeyEvent(int eventCode) const;
		void injectRelEvent(int xCoord, int yCoord) const;
		void unregisterEventDevice();
		bool checkAlive() const;
		void sendViaSocket(std::string const& message);

		int getID() const;
		sf::IpAddress const getIP();
		sf::TcpSocket* getSocket();

		static int instance_count;


	private:
		int id_;
		sf::TcpSocket* socket_;
		int uinputHandle_;
		uinput_user_dev eventDevice_;
};

#endif //PLAYER_CONNECTION_HPP