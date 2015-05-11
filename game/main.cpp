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


	sf::RenderWindow App(sf::VideoMode(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().width,20), "MMMBall");

	App.setFramerateLimit(60);
	
	Game* game = new Game();
	game->setScreenWidth(sf::VideoMode::getDesktopMode().width);
	game->setScreenHeight(sf::VideoMode::getDesktopMode().height);
	game->createField();
	
	// MMM Input device recognizer and grabber
	InputHandler inputHandler(game);

	int frameCounterMod(0);
	float lastFramerate(0.0);

	sf::Event event;

	while (App.isOpen()) 
	{
		App.clear(sf::Color(255,255,0));

		//handle events - do not use App.pollEvent(..) as it slows framerate on mouse move
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			App.close();
		}

		// input handling
		inputHandler.processAddRemoveQueries();
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
		
		frameCounterMod = (frameCounterMod + 1) % 5;
		
		if (frameCounterMod == 0)
		{
			float framerate = 5.0f/Clock.getElapsedTime().asSeconds();
			Clock.restart();
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
