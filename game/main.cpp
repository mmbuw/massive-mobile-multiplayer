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
	game.loadBackground();
	game.initPlayers();
	while (App.IsOpened())
	{
		float Framerate = 1.f / Clock.GetElapsedTime();
		std::cout<<Framerate<<"FPS"<<std::endl;
    		Clock.Reset();

		sf::Event Event;
		while (App.GetEvent(Event)) 
		{
			if (Event.Type == sf::Event::Closed){
				App.Close();
			}

			
			if (input.IsKeyDown(sf::Key::Left)){
				game.movePlayer(0,"LEFT");			
			}
			if (input.IsKeyDown(sf::Key::Right)){
				game.movePlayer(0,"RIGHT");			
			}
			if (input.IsKeyDown(sf::Key::Up)){
				game.movePlayer(0,"UP");			
			}
			if (input.IsKeyDown(sf::Key::Down)){
				game.movePlayer(0,"DOWN");			
			}
			
			if (input.IsKeyDown(sf::Key::W)){
				game.movePlayer(1,"UP");			
			}
			if (input.IsKeyDown(sf::Key::A)){
				game.movePlayer(1,"LEFT");			
			}
			if (input.IsKeyDown(sf::Key::S)){
				game.movePlayer(1,"DOWN");			
			}
			if (input.IsKeyDown(sf::Key::D)){
				game.movePlayer(1,"RIGHT");			
			}

		}

		App.Clear();
		game.renderBackground(&App);
		game.renderPlayers(&App);

		App.Display();
	}

	return EXIT_SUCCESS;
}
