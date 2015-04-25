#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.hpp"

int main()
{
	sf::Clock Clock;
	sf::RenderWindow App(sf::VideoMode(1920,1350,32), "MMMBall");
	const sf::Input& input = App.GetInput();
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

	while (App.IsOpened()) 
	{
		sf::Event Event;
		App.Clear(sf::Color(255,0,0));

		// handle events
		while (App.GetEvent(Event)) 
		{
			if (Event.Type == sf::Event::Closed || input.IsKeyDown(sf::Key::Escape))
			{ 
				App.Close();
			}
			
			if (input.IsKeyDown(sf::Key::J) && jPressed == false)
			{
				jPressed = true;
			}
			else if (input.IsKeyDown(sf::Key::J) == false && jPressed == true)
			{
				jPressed = false;
			}

			if (input.IsKeyDown(sf::Key::L) && lPressed == false)
			{
				lPressed = true;		
			}
			else if (input.IsKeyDown(sf::Key::L) == false && lPressed == true)
			{
				lPressed = false;
			}

			if (input.IsKeyDown(sf::Key::I) && iPressed == false)
			{
				iPressed = true;
			}
			else if (input.IsKeyDown(sf::Key::I) == false && iPressed == true)
			{
				iPressed = false;
			}

			if (input.IsKeyDown(sf::Key::K) && kPressed == false)
			{
				kPressed = true;		
			}
			else if (input.IsKeyDown(sf::Key::K) == false && kPressed == true)
			{
				kPressed = false;
			}
			
			if (input.IsKeyDown(sf::Key::W) && wPressed == false)
			{
				wPressed = true;	
			}
			else if (input.IsKeyDown(sf::Key::W) == false && wPressed == true)
			{
				wPressed = false;
			}

			if (input.IsKeyDown(sf::Key::A) && aPressed == false)
			{
				aPressed = true;		
			}
			else if (input.IsKeyDown(sf::Key::A) == false && aPressed == true)
			{
				aPressed = false;
			}

			if (input.IsKeyDown(sf::Key::S) && sPressed == false)
			{
				sPressed = true;
			}
			else if (input.IsKeyDown(sf::Key::S) == false && sPressed == true)
			{
				sPressed = false;
			}

			if (input.IsKeyDown(sf::Key::D) && dPressed == false)
			{
				dPressed = true;
			}
			else if (input.IsKeyDown(sf::Key::D) == false && dPressed == true)
			{
				dPressed = false;
			}

			if (input.IsKeyDown(sf::Key::E) && ePressed == false)
			{
				ePressed = true;
			}

			if (input.IsKeyDown(sf::Key::O) && oPressed == false)
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

		float framerate = 1.0f/Clock.GetElapsedTime();
		Clock.Reset();
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

		App.Display();
	}

	return EXIT_SUCCESS;
}
