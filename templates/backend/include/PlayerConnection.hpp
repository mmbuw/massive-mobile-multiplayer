#ifndef PLAYER_CONNECTION_HPP
#define PLAYER_CONNECTION_HPP

//SFML
#include <SFML/Network.hpp>

//UInput dependencies
#include <linux/input.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>

//Application
#include "ConfigurationFileParser.hpp"

//C++
#include <iostream>
#include <cstring>
#include <sstream>
#include <chrono>

#define CONNECTION_TIMEOUT_SECONDS 30

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::seconds seconds;

class PlayerConnection
{
	public: 
		PlayerConnection(sf::TcpSocket* socket);
		~PlayerConnection();

		void injectSingleEvent(int type, int code, int value) const;
		void injectMultiEvent(int type, std::vector<int> const& codes, std::vector<int> const& values) const;
		void createEventDevice();
		void unregisterEventDevice();
		bool checkAlive() const;
		void sendViaSocket(std::string const& message);

		int getID() const;
		sf::IpAddress const getIP();
		sf::TcpSocket* getSocket();
		std::string const getName();

		static int instance_count;
		static RegisteredInputs registeredInputs;


	private:
		int id_;
		sf::TcpSocket* socket_;
		int uinputHandle_;
		uinput_user_dev eventDevice_;

		mutable Clock::time_point lastInputTime_;

};

#endif //PLAYER_CONNECTION_HPP