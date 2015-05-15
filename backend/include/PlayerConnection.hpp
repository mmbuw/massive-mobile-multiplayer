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
#include <chrono>

#define CONNECTION_TIMEOUT_SECONDS 500

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::seconds seconds;

class PlayerConnection
{
	public: 
		PlayerConnection(sf::TcpSocket* socket);
		~PlayerConnection();

		void injectKeyEvent(int eventCode) const;
		void injectRelEvent(int xCoord, int yCoord) const;
		void createEventDevice();
		void unregisterEventDevice();
		bool checkAlive() const;
		void sendViaSocket(std::string const& message);

		int getID() const;
		sf::IpAddress const getIP();
		sf::TcpSocket* getSocket();
		std::string const getName();
		void setName(std::string const& name);

		static int instance_count;


	private:
		int id_;
		std::string name_;
		sf::TcpSocket* socket_;
		int uinputHandle_;
		uinput_user_dev eventDevice_;

		mutable Clock::time_point lastInputTime_;

};

#endif //PLAYER_CONNECTION_HPP