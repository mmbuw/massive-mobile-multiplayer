#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <map>
#include <bitset>

#include "PlayerConnection.hpp"
#include "ConnectionDatabase.hpp"
#include "base64.hpp"
#include "sha1.hpp"


int main()
{
	bool running(true);
	int port(53000);
	int globalConnectionCounter(0);
	ConnectionDatabase currentConnections;

	//create a socket listener
	sf::SocketTCP listener;
	listener.Listen(53000);

	sf::Selector<sf::SocketTCP> selector;
	selector.Add(listener);

	//information needed for WebSocket handshake (RFC 6455)
	std::string globallyUniqueIdentifier("258EAFA5-E914-47DA-95CA-C5AB0DC85B11"); //RFC 4122
	std::string requestSearchString("Sec-WebSocket-Key: ");

	std::cout << std::endl;
	std::cout << "#####################################" << std::endl;
	std::cout << "# MMM - Game Controller Server         " << std::endl;
	std::cout << "# Startup successful (port " << port << ")"<< std::endl;
	std::cout << "#####################################" << std::endl;
	std::cout << std::endl;


	while(running)
	{
		/*
		//clean up all timed out connections
		for (std::map<sf::SocketTCP, PlayerConnection*>::iterator it = socketPlayerConnections.begin(); it != socketPlayerConnections.end();)
		{
			if ((*it).second->isResponding() == false)
			{
	            std::cout << "[Cleanup] " << (*it).second->ip_ << " (Client ID " << (*it).second->id_ << ") is removed in main thread." << std::endl;

		        selector.Remove((*it).first);
		        delete (*it).second;
		        it = socketPlayerConnections.erase(it);
			}
			else
			{
			    ++it;
			}
		}*/

		//wait until at least one socket has news
		unsigned int nbSockets = selector.Wait();

		//iterate over all sockets to process
		for (unsigned int i = 0; i < nbSockets; ++i)
		{
			sf::SocketTCP socket = selector.GetSocketReady(i);

			//if the listening socket is ready, we can accept a new connection
			if (socket == listener)
		    {
		        sf::IPAddress address;
		        sf::SocketTCP client;
		        listener.Accept(client, &address);

		        std::stringstream responseStream;

		        //check for multiple connections
		        if (currentConnections.get_player_connection(address) != nullptr)
		        {
					responseStream << "HTTP/1.1 403 Forbidden\r\n\r\n";
		        }
		        else
		        {
			        //add the new socket to the selector
			        ++globalConnectionCounter;
			        PlayerConnection* playerConnection = new PlayerConnection(globalConnectionCounter, address, client);
			        currentConnections.put_elements(client, playerConnection);
			        selector.Add(client);

			    	
			    	/* Perform WebSocket handshake (RFC 6455) */

			    	//receive request
			    	char requestBuffer[10000];
			    	std::size_t receivedSize;
			    	
			    	if (client.Receive(requestBuffer, sizeof(requestBuffer), receivedSize) != sf::Socket::Done)
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
			    	std::cout << "[Connect] " << address << " (Client ID " << globalConnectionCounter << ")" << std::endl;
			    }

			    std::string responseStreamString = responseStream.str();
		    	char responseStreamBuffer[responseStreamString.size()];
		    	
		    	for (int i = 0; i < responseStreamString.size(); ++i)
		    	{
		    		responseStreamBuffer[i] = responseStreamString[i];
		    	}

		    	if (client.Send(responseStreamBuffer, sizeof(responseStreamBuffer)) != sf::Socket::Done)
		    	{
		    		std::cout << "[Error] Sending responseStream failed." << std::endl;
		    	}

		    }

		    //else it is a client socket, so we read the message which was sent
		    else
		    {
		        char receiveBuffer[50];
		    	std::size_t receiveSize;
		    	bool performCleanup(false);

		        if (socket.Receive(receiveBuffer, sizeof(receiveBuffer), receiveSize) == sf::Socket::Done)
		        {
		        	//get client information using its PlayerConnection instance
		            PlayerConnection* playerConnection = currentConnections.get_player_connection(socket);

		            /* Decoding the client message using the WebSocket protocol */
		            /* (http://stackoverflow.com/questions/8125507/how-can-i-send-and-receive-websocket-messages-on-the-server-side) */

		            //the bit masks used for decode WebSocket messages
		            //unfortunately cannot be stored in a std::vector for some reason
		            std::bitset<8> mask_0;
		            std::bitset<8> mask_1;
		            std::bitset<8> mask_2;
		            std::bitset<8> mask_3;

            		//omit leading 1 in second byte (length)
		            int indexOfFirstMask(2);
            		std::bitset<8> length = ((std::bitset<8>) receiveBuffer[i] ^= std::bitset<8>(std::string("01111111")));
            		unsigned long lengthLong = length.to_ulong();

            		if (lengthLong == 126)
            			indexOfFirstMask = 4;
            		else if (lengthLong == 127)
            			indexOfFirstMask = 10;

            		int indexOfFirstDataByte(indexOfFirstMask+4);
            		int numDataBytes(receiveSize-indexOfFirstDataByte);
		            unsigned long decodedLong[numDataBytes];

		            //get decoding masks
		            mask_0 = (std::bitset<8>) receiveBuffer[indexOfFirstMask];
		            mask_1 = (std::bitset<8>) receiveBuffer[indexOfFirstMask+1];
		            mask_2 = (std::bitset<8>) receiveBuffer[indexOfFirstMask+2];
		            mask_3 = (std::bitset<8>) receiveBuffer[indexOfFirstMask+3];

		            //decode data bytes with respective mask
		            for (int i = indexOfFirstDataByte; i < receiveSize; ++i)
		            {
		            	if ( (i-indexOfFirstDataByte) % 4 == 0)
		            		decodedLong[i-indexOfFirstDataByte] = ((std::bitset<8>) receiveBuffer[i] ^= mask_0).to_ulong();
		            	else if ( (i-indexOfFirstDataByte) % 4 == 1)
		            		decodedLong[i-indexOfFirstDataByte] = ((std::bitset<8>) receiveBuffer[i] ^= mask_1).to_ulong();
		            	else if ( (i-indexOfFirstDataByte) % 4 == 2)
		            		decodedLong[i-indexOfFirstDataByte] = ((std::bitset<8>) receiveBuffer[i] ^= mask_2).to_ulong();
		            	else
		            		decodedLong[i-indexOfFirstDataByte] = ((std::bitset<8>) receiveBuffer[i] ^= mask_3).to_ulong();
		            }

		            //detect refresh of page
		            if (numDataBytes == 2 && decodedLong[0] == 3 && decodedLong[1] == 233)
		            {
		            	performCleanup = true;
		            }
		            else
		            {
			            //convert decoded long values to characters
			            std::string message("");

			            for (int i = 0; i < numDataBytes; ++i)
			            {
			            	message += (char) decodedLong[i];
			            }

			            //print message
			            //std::cout << "[Client " << playerConnection->id_ << "]: " << message << std::endl;

			            //react on messages by injecting keystrokes
			            if (message == "A")
			            {
			            	playerConnection->injectKeyEvent(BTN_A);
			            }
			            else
			            {
			            	std::stringstream stream(message);
			            	int x, y;

			            	stream >> x;
			            	stream >> y;

							//std::cout << "X: " << x << std::endl;
							//std::cout << "Y: " << y << std::endl;

			            	if (x <= 1024 && y <= 1024 && x >= 0 && y >= 0)
			            		playerConnection->injectRelEvent(x, y);
			            }

			        }
			        
		        }
		        else
		        {
		        	performCleanup = true;
		        }


		        if (performCleanup)
		        {
		            //the connection is lost, perform cleanup
		            PlayerConnection* playerConnection = currentConnections.get_player_connection(socket);
		            std::cout << "[Disconnect] " << playerConnection->ip_ << " (Client ID " << playerConnection->id_ << ")" << std::endl;

		            currentConnections.remove_element(socket);
		            delete playerConnection;
		            selector.Remove(socket);
		        }
		    }

		}

	}

    return 0;
}
