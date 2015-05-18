#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#include "InputHandler.hpp"
#include "Game.hpp"

#define INSERT_KEYBOARD_DEBUG_PLAYER false

int main()
{
	srand (time(NULL));
	sf::Clock Clock;


	sf::RenderWindow App(sf::VideoMode(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().width,20), "MMMBall");

	App.setFramerateLimit(60);
	App.setVerticalSyncEnabled(true);
	
	Game* game = new Game();
	game->setScreenWidth(sf::VideoMode::getDesktopMode().width);
	game->setScreenHeight(sf::VideoMode::getDesktopMode().height);
	game->createField();
	
	// MMM Input device recognizer and grabber
	InputHandler inputHandler(game);

	int frameCounterMod(0);
	float lastFramerate(0.0);

	sf::Event event;

	//debug keyboard player
	#if INSERT_KEYBOARD_DEBUG_PLAYER
	    Player* debugPlayer = game->addNewPlayer("Keyboard", 29);
		bool wPressed(false);
		bool aPressed(false);
		bool sPressed(false);
		bool dPressed(false);
	#endif

	while (App.isOpen()) 
	{
		App.clear(sf::Color(255,255,0));

		//handle events - do not use App.pollEvent(..) as it slows framerate on mouse move
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			App.close();
		}

		//handle keyboard debug player inputs
		#if INSERT_KEYBOARD_DEBUG_PLAYER
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && wPressed == false)
			{
				wPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) == false && wPressed == true)
			{
				wPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && aPressed == false)
			{
				aPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) == false && aPressed == true)
			{
				aPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sPressed == false)
			{
				sPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) == false && sPressed == true)
			{
				sPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && dPressed == false)
			{
				dPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) == false && dPressed == true)
			{
				dPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				debugPlayer->shoot();
			}


			if (wPressed)
				debugPlayer->moveUp();
			if (aPressed)
				debugPlayer->moveLeft();
			if (sPressed)
				debugPlayer->moveDown();
			if (dPressed)
				debugPlayer->moveRight();
		#endif

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
