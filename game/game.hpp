#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.hpp"
#include "ball.hpp"


class Game {
	
	public: 
		void initPlayers();
		void initBall();
	
		void renderBackground(sf::RenderWindow* window);
		void renderPlayers(sf::RenderWindow* window);		
		void renderBall(sf::RenderWindow* window);
		void renderSidelines(sf::RenderWindow* window);		

		void addPlayer();

		void movePlayer(int playerNumber, std::string direction);


	private:
		std::vector<Player> players;
		sf::Image background;
		Ball ball;	

};
