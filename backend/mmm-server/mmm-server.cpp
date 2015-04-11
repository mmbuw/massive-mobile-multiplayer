#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <iostream>
#include <map>

#include "PlayerConnection.hpp"
#include "base64.hpp"
#include "sha1.hpp"

int main()
{
	bool running(true);
	int globalConnectionCounter(0);
	std::map<sf::SocketTCP, PlayerConnection*> socketPlayerConnetions;

	//create a socket listener
	sf::SocketTCP listener;
	listener.Listen(53000);

	sf::SelectorTCP selector;
	selector.Add(listener);

	while(running)
	{
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

		        //add the new socket to the selector
		        ++globalConnectionCounter;
		        PlayerConnection* playerConnection = new PlayerConnection(globalConnectionCounter, address, client);
		        socketPlayerConnetions.insert(std::make_pair(client, playerConnection));
		        selector.Add(client);
		    	
		    	/* Perform WebSocket handshake */

		    	//receive request
		    	char requestBuffer[1024];
		    	std::size_t receivedSize;
		    	
		    	if (client.Receive(requestBuffer, sizeof(requestBuffer), receivedSize) != sf::Socket::Done)
		    	{
		    		std::cout << "Error: Receiving request failed." << std::endl;
		    	}

		    	std::string request(requestBuffer);

		    	//find Sec-WebSocket-Key
		    	std::string searchString("Sec-WebSocket-Key: ");

		    	std::size_t stringPosition = request.find(searchString);
		    	std::size_t endPosition = request.find("\r\n", stringPosition);
		    	std::string socketKey = request.substr(stringPosition + searchString.size(), 
		    		                                   endPosition-stringPosition-searchString.size());

		    	//generate Sec-WebSocket-Accept
		    	std::string globallyUniqueIdentifier("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
		    	std::stringstream concatStream;
		    	concatStream << socketKey << globallyUniqueIdentifier;
		    	std::string concatenatedString = concatStream.str();

	    		unsigned char hash[20];
				sha1::calc(concatenatedString.c_str(), concatenatedString.size(), hash);
				std::string secWebSocketAccept = base64_encode(reinterpret_cast<const unsigned char*>(hash), 20);
				
				//send acceptance response
				std::stringstream response;
				response << "HTTP/1.1 101 Switching Protocols\r\n";
				response << "Upgrade: websocket\r\n";
				response << "Connection: Upgrade\r\n";
				response << "Sec-WebSocket-Accept: " << secWebSocketAccept << "\r\n\r\n";

				std::string responseString = response.str();
		    	char responseBuffer[responseString.size()]; 
		    	
		    	for (int i = 0; i < responseString.size(); ++i)
		    	{
		    		responseBuffer[i] = responseString[i];
		    	}

		    	if (client.Send(responseBuffer, sizeof(responseBuffer)) != sf::Socket::Done)
		    	{
		    		std::cout << "Error: Sending response failed." << std::endl;
		    	}

		    	//client is now connected
		    	std::cout << "Client " << globalConnectionCounter << " (" << address << ") connected." << std::endl;

		    }

		    //else it is a client socket, so we read the message which was sent
		    else
		    {
		        sf::Packet packet;

		        if (socket.Receive(packet) == sf::Socket::Done)
		        {
		         	//extract the message and print it
		            std::string message;
		            packet >> message;

		            PlayerConnection* playerConnection = socketPlayerConnetions.find(socket)->second;

		            std::cout << "[Client " << playerConnection->id_ << "]: \"" << message << "\"" << std::endl;

		            //react on messages by injecting keystrokes
		            //playerConnection->injectKeyEvent(BTN_A);
		            playerConnection->injectRelEvent(100,100);
		        }
		        else
		        {
		            //the connection is lost, perform cleanup
		            std::map<sf::SocketTCP, PlayerConnection*>::iterator mapIteratorToDelete = socketPlayerConnetions.find(socket);
		            std::cout << "Client " << mapIteratorToDelete->second->id_ << " disconnected." << std::endl;

		            delete mapIteratorToDelete->second;
		            socketPlayerConnetions.erase(mapIteratorToDelete);
		            selector.Remove(socket);
		        }
		    }

		}

	}

    return 0;
}