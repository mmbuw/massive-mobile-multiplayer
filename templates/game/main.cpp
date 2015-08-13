#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "InputHandler.hpp"

int main()
{
	srand (time(NULL));
	sf::Clock Clock;

	sf::RenderWindow window(sf::VideoMode(800, 600, 20), "Application Template");
	window.setVerticalSyncEnabled(true);
	
	// MMM Input device recognizer and grabber
	InputHandler inputHandler;

	//main loop
	while (window.isOpen()) 
	{
		// clear window
		window.clear(sf::Color(0,0,0));

		// input handling
		inputHandler.processAddRemoveQueries();
		inputHandler.processDeviceInputs();
		
		/* TODO for application developer: implement application logic */

		// refresh window
		window.display();
	}

	return EXIT_SUCCESS;
}
