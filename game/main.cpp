#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.hpp"
#include "game.hpp"

int main()
{
	sf::Clock Clock;
	sf::RenderWindow App(sf::VideoMode(1920,1200,32), "MMMBall");
	const sf::Input& input = App.GetInput();
	Game game;	
	game.initPlayers();

	bool jPressed(false);
	bool lPressed(false);
	bool iPressed(false);
	bool kPressed(false);
	bool wPressed(false);
	bool aPressed(false);
	bool sPressed(false);
	bool dPressed(false);

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

		}

		//handle key events according to flags set above
		if (jPressed)
		{
			game.movePlayer(0,"LEFT");
		}

		if (lPressed)
		{
			game.movePlayer(0,"RIGHT");
		}

		if (iPressed)
		{
			game.movePlayer(0,"UP");
		}

		if (kPressed)
		{
			game.movePlayer(0,"DOWN");
		}

		if (wPressed)
		{
			game.movePlayer(1,"UP");
		}

		if (aPressed)
		{
			game.movePlayer(1,"LEFT");
		}

		if (sPressed)
		{
			game.movePlayer(1,"DOWN");
		}

		if (dPressed)
		{
			game.movePlayer(1,"RIGHT");
		}
		
		// application logic
		game.updatePhysicalObjects();
		game.checkIntersect();
		game.checkForGoal();
		

		// render output
		game.renderBackground(&App);
		game.renderGoals(&App);
		game.renderSidelines(&App);
		game.renderPlayers(&App);
		game.renderBall(&App);	
		
		App.Display();
	}

	return EXIT_SUCCESS;
}
