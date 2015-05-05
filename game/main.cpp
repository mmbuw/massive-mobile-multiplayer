#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#include "InputHandler.hpp"
#include "Game.hpp"

int main()
{
	  srand (time(NULL));
	sf::Clock Clock;

	sf::RenderWindow App(sf::VideoMode(1920,1350,20), "MMMBall");
	App.setSize(sf::Vector2u(1920,1350));

	
	Game* game = new Game();
	// MMM Input device recognizer and grabber
	InputHandler inputHandler(game);

	int frameCounterMod(0);
	float lastFramerate(0.0);

	while (App.isOpen()) 
	{
		sf::Event event;
		App.clear(sf::Color(255,0,0));

		// handle events
		while (App.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
			{ 
				App.close();
			}
		}

		// input handling
		inputHandler.updateDeviceList();
		inputHandler.processDeviceInputs();
		

		// application logic
		game->updatePhysicalObjects();
		game->applyIntersectionPhysics();
		game->checkForGoal();


		// render output
		game->renderBackground(&App);
		game->renderGoals(&App);
		game->renderSidelines(&App);
		game->renderPlayers(&App);
		game->renderBall(&App);
		game->renderScoreLine(&App);

		float framerate = 1.0f/Clock.getElapsedTime().asSeconds();
		Clock.restart();
		frameCounterMod = (frameCounterMod + 1) % 5;
		
		if (frameCounterMod == 0)
		{
			game->renderFpsDisplay(&App, framerate);
			lastFramerate = framerate;
		}
		else
		{
			game->renderFpsDisplay(&App, lastFramerate);
		}

		App.display();
	}

	delete game;

	return EXIT_SUCCESS;
}
