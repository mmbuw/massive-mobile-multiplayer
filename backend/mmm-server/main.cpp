//SFML
#include <SFML/Network.hpp>

//C++
#include <iostream>
#include <map>
#include <thread>
#include <chrono>
#include <bitset>

//Application
#include "PlayerConnection.hpp"
#include "sha1.hpp"
#include "base64.hpp"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::nanoseconds nanoseconds;

int tickFPS(60);
int port(53000);

int main()
{
	bool running(true);
	std::map<int, PlayerConnection*> currentPlayerConnections;
	int frameDurationNanoSec(1.0/tickFPS * 1000000000);

	//information needed for WebSocket handshake (RFC 6455)
	std::string globallyUniqueIdentifier("258EAFA5-E914-47DA-95CA-C5AB0DC85B11"); //RFC 4122
	std::string requestSearchString("Sec-WebSocket-Key: ");

	/* set up network communication */
	sf::TcpListener listener;
	listener.setBlocking(false);

	//bind the listener to a port
	if (listener.listen(port) != sf::Socket::Done)
	{
		std::cout << "[Server] Error while binding the listener port " << port << "." << std::endl;
		return 1;
	}

	std::cout << std::endl;
	std::cout << "#####################################" << std::endl;
	std::cout << "# MMM - Game Controller Server         " << std::endl;
	std::cout << "# Startup successful (port " << port << ")"<< std::endl;
	std::cout << "#####################################" << std::endl;
	std::cout << std::endl;

	Clock::time_point lastFrameTime = Clock::now();

	while (running)
	{
		//check for a new connection
		sf::TcpSocket* newClientSocket = new sf::TcpSocket;
		newClientSocket->setBlocking(false);
		bool deleteNewClientSocket(false);

		if (listener.accept(*newClientSocket) == sf::Socket::Done)
		{
			std::stringstream responseStream;

			//do not allow multiple connections by same host
			if (currentPlayerConnections.find(newClientSocket->getRemoteAddress().toInteger()) != currentPlayerConnections.end())
			{
				responseStream << "HTTP/1.1 403 Forbidden\r\n\r\n";
				deleteNewClientSocket = true;
			}
			else
			{  	
		        PlayerConnection* playerConnection = new PlayerConnection(newClientSocket);
		    	
		    	/* Perform WebSocket handshake (RFC 6455) */

		    	//receive request
		    	char requestBuffer[10000];
		    	std::size_t receivedSize;
		    	
		    	if (newClientSocket->receive(requestBuffer, sizeof(requestBuffer), receivedSize) != sf::Socket::Done)
		    	{
		    		std::cout << "[Error] Receiving request failed." << std::endl;
		    	}

		    	std::string request(requestBuffer);

		    	//std::cout << request << std::endl;

		    	//find Sec-WebSocket-Key
		    	std::size_t stringPosition = request.find(requestSearchString);
		    	std::size_t endPosition = request.find("\r\n", stringPosition);
		    	std::string secWebSocketKey = request.substr(stringPosition + requestSearchString.size(), 
		    		                                         endPosition-stringPosition-requestSearchString.size());

		    	//generate Sec-WebSocket-Accept
		    	std::string concatenatedString = secWebSocketKey + globallyUniqueIdentifier;

	    		unsigned char sha1Hash[20];
				sha1::calc(concatenatedString.c_str(), concatenatedString.size(), sha1Hash);
				std::string secWebSocketAccept = base64_encode(reinterpret_cast<const unsigned char*>(sha1Hash), 20);
				
				//send acceptance response
				responseStream << "HTTP/1.1 101 Switching Protocols\r\n";
				responseStream << "Upgrade: websocket\r\n";
				responseStream << "Connection: Upgrade\r\n";
				responseStream << "Sec-WebSocket-Accept: " << secWebSocketAccept << "\r\n\r\n";

		    	//client is now connected
		    	std::cout << "[Connect] " << playerConnection->getIP() << " (Client ID " << playerConnection->getID() << ")" << std::endl;
		    	currentPlayerConnections.insert(std::make_pair(playerConnection->getIP().toInteger(), playerConnection));
		    }

		    //send response
		    std::string responseStreamString = responseStream.str();
	    	char responseStreamBuffer[responseStreamString.size()];
	    	
	    	for (int i = 0; i < responseStreamString.size(); ++i)
	    	{
	    		responseStreamBuffer[i] = responseStreamString[i];
	    	}

	    	if (newClientSocket->send(responseStreamBuffer, sizeof(responseStreamBuffer)) != sf::Socket::Done)
	    	{
	    		std::cout << "[Error] Sending responseStream failed." << std::endl;
	    	}

		}
		else
		{
			//no new connection came in using newClientSocket
			deleteNewClientSocket = true;
		}

		if (deleteNewClientSocket)
		{
			delete newClientSocket;
		}

		//check for new message and disconnection at each socket
		for (std::map<int, PlayerConnection*>::iterator it = currentPlayerConnections.begin(); it != currentPlayerConnections.end(); )
		{
			PlayerConnection* playerConnection = it->second;

			char receiveBuffer[100];
	    	std::size_t receiveSize;
			sf::Socket::Status status = playerConnection->getSocket()->receive(receiveBuffer, sizeof(receiveBuffer), receiveSize);

			if (playerConnection->checkAlive() == false)
			{
				std::cout << "[Timeout] " << playerConnection->getSocket()->getRemoteAddress() << " (Client " << playerConnection->getID() << ")" << std::endl;
				delete playerConnection;
				it = currentPlayerConnections.erase(it);
			}
			else if (status == sf::Socket::NotReady)
			{
				++it;
				continue;
			}
			else if (status == sf::Socket::Disconnected)
			{
				std::cout << "[Disconnect] " << playerConnection->getSocket()->getRemoteAddress() << " (Client " << playerConnection->getID() << ")" << std::endl;
				delete playerConnection;
				it = currentPlayerConnections.erase(it);
			}
			else
			{
				bool nextMessagePresent(false);

	            int indexOfFirstMask(2);
        		int indexOfFirstDataByte(indexOfFirstMask+4);
        		int numDataBytes(receiveSize-indexOfFirstDataByte);

				do
				{
			    	/* Decoding the client message using the WebSocket protocol */
		            /* (http://stackoverflow.com/questions/8125507/how-can-i-send-and-receive-websocket-messages-on-the-server-side) */

		            //the bit masks used for decode WebSocket messages
		            //unfortunately cannot be stored in a std::vector for some reason
		            std::bitset<8> mask_0;
		            std::bitset<8> mask_1;
		            std::bitset<8> mask_2;
		            std::bitset<8> mask_3;

		            //get decoding masks
		            mask_0 = (std::bitset<8>) receiveBuffer[indexOfFirstMask];
		            mask_1 = (std::bitset<8>) receiveBuffer[indexOfFirstMask+1];
		            mask_2 = (std::bitset<8>) receiveBuffer[indexOfFirstMask+2];
		            mask_3 = (std::bitset<8>) receiveBuffer[indexOfFirstMask+3];

		            std::string message("");

		            //decode data bytes with respective mask
		            for (int i = indexOfFirstDataByte; i < receiveSize; ++i)
		            {
		            	long characterAsLong;

		            	if ( (i-indexOfFirstDataByte) % 4 == 0)
		            		characterAsLong = ((std::bitset<8>) receiveBuffer[i] ^= mask_0).to_ulong();
		            	else if ( (i-indexOfFirstDataByte) % 4 == 1)
		            		characterAsLong = ((std::bitset<8>) receiveBuffer[i] ^= mask_1).to_ulong();
		            	else if ( (i-indexOfFirstDataByte) % 4 == 2)
		            		characterAsLong = ((std::bitset<8>) receiveBuffer[i] ^= mask_2).to_ulong();
		            	else
		            		characterAsLong = ((std::bitset<8>) receiveBuffer[i] ^= mask_3).to_ulong();
		            
		            	//check for end of message sign (sign .)
		            	if (characterAsLong == (long) '.' && i != receiveSize-1)
		            	{
		            		nextMessagePresent = true;
		            		indexOfFirstMask = i + 3;
							indexOfFirstDataByte = indexOfFirstMask + 4;
        					numDataBytes = receiveSize-indexOfFirstDataByte;
        					message += (char) characterAsLong;
		            		break;
		            	}
		            	else
		            	{
			            	nextMessagePresent = false;
			            	message += (char) characterAsLong;
		            	}

		            }

		            //print message
		            //std::cout << "[Client " << playerConnection->getID() << "] " << message << std::endl;

		            //react on messages by injecting keystrokes
		            if (message == "VAL A$")
		            {
		            	//std::cout << "Inject key input event for A" << std::endl;
		            	playerConnection->injectKeyEvent(BTN_A);
		            }
		            else if (message.find("NAME") == 0)
		            {
		            	std::stringstream stream(message);
		            	std::string nameString, nameToSet;

		            	stream >> nameString;
		            	stream >> nameToSet;

		            	std::cout << "[Client " << playerConnection->getID() << "] Assigning name: " << nameToSet.substr(0, nameToSet.size()-1) << std::endl;
		            	playerConnection->setName(nameToSet.substr(0, nameToSet.size()-1));
		            }
		            else if (message.find("VAL") == 0)
		            {
		            	std::stringstream stream(message);
		            	std::string nameString;
		            	int x, y;

		            	stream >> nameString;
		            	stream >> x;
		            	stream >> y;

		            	if (x <= 1024 && y <= 1024 && x >= 0 && y >= 0)
		            	{
		            		playerConnection->injectRelEvent(x, y);
		            		//std::cout << "Inject relative input event" << std::endl;
		            	}
		            }
		            else
		            {
		            	std::cout << "[Client " << playerConnection->getID() << "] Omitting invalid message: " << message << std::endl;
		            	nextMessagePresent = false;
		            }

			    } while (nextMessagePresent);

		        //move to next socket
				++it;
			}

		}

		//slow the application down to tickFPS
		Clock::time_point end_time = Clock::now();
		int currentNanoSec = (std::chrono::duration_cast<nanoseconds>(end_time - lastFrameTime)).count();
		std::this_thread::sleep_for(std::chrono::nanoseconds(frameDurationNanoSec - currentNanoSec));

		//fps output
		//int frameDuration = (std::chrono::duration_cast<nanoseconds>(Clock::now() - lastFrameTime)).count();
		//std::cout << "FPS: " << 1000000000.0/frameDuration << std::endl;

		lastFrameTime = Clock::now();
	}

	return 0;
}