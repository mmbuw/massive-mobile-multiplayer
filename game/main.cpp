#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>

#include "Game.hpp"

int main()
{
	  srand (time(NULL));
	sf::Clock Clock;
	sf::RenderWindow App(sf::VideoMode(1920,1350,20), "MMMBall");
	App.setVerticalSyncEnabled(false);
	App.setFramerateLimit(60);
	App.setSize(sf::Vector2u(1920,1350));
	Game game;

	bool jPressed(false);
	bool lPressed(false);
	bool iPressed(false);
	bool kPressed(false);
	bool wPressed(false);
	bool aPressed(false);
	bool sPressed(false);
	bool dPressed(false);

	bool ePressed(false);
	bool oPressed(false);

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
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)  && jPressed == false)
			{
				jPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J) == false && jPressed == true)
			{
				jPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)  && lPressed == false)
			{
				lPressed = true;		
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)  == false && lPressed == true)
			{
				lPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)  && iPressed == false)
			{
				iPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)  == false && iPressed == true)
			{
				iPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)  && kPressed == false)
			{
				kPressed = true;		
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) == false && kPressed == true)
			{
				kPressed = false;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)  && wPressed == false)
			{
				wPressed = true;	
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)  == false && wPressed == true)
			{
				wPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)  && aPressed == false)
			{
				aPressed = true;		
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)  == false && aPressed == true)
			{
				aPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)  && sPressed == false)
			{
				sPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)  == false && sPressed == true)
			{
				sPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)  && dPressed == false)
			{
				dPressed = true;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)  == false && dPressed == true)
			{
				dPressed = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)  && ePressed == false)
			{
				ePressed = true;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && oPressed == false)
			{
				oPressed = true;
			}

		}

		//handle key events according to flags set above
		if (jPressed)
		{
			game.movePlayer(1, "LEFT");
		}

		if (lPressed)
		{
			game.movePlayer(1, "RIGHT");
		}

		if (iPressed)
		{
			game.movePlayer(1, "UP");
		}

		if (kPressed)
		{
			game.movePlayer(1, "DOWN");
		}

		if (wPressed)
		{
			game.movePlayer(0, "UP");
		}

		if (aPressed)
		{
			game.movePlayer(0, "LEFT");
		}

		if (sPressed)
		{
			game.movePlayer(0, "DOWN");
		}

		if (dPressed)
		{
			game.movePlayer(0, "RIGHT");
		}

		if (ePressed)
		{
			game.playerShoot(0);
			ePressed = false;
		}

		if (oPressed)
		{
			game.playerShoot(1);
			oPressed = false;
		}
		
		
		// application logic
		game.updatePhysicalObjects();
		game.applyIntersectionPhysics();
		game.checkForGoal();
		

		// render output
		game.renderBackground(&App);
		game.renderGoals(&App);
		game.renderSidelines(&App);
		game.renderPlayers(&App);
		game.renderBall(&App);
		game.renderScoreLine(&App);

		float framerate = 1.0f/Clock.getElapsedTime().asSeconds();
		Clock.restart();
		frameCounterMod = (frameCounterMod + 1) % 5;
		
		if (frameCounterMod == 0)
		{
			game.renderFpsDisplay(&App, framerate);
			lastFramerate = framerate;
		}
		else
		{
			game.renderFpsDisplay(&App, lastFramerate);
		}

		App.display();
	}

	return EXIT_SUCCESS;
}
