#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "player.hpp"
#include "ball.hpp"
#include "PhysicalObject.hpp"


class Game 
{
	
	public:
		Game();

		void initPlayers();
		void initBall();
	
		void renderBackground(sf::RenderWindow* window);
		void renderPlayers(sf::RenderWindow* window);		
		void renderBall(sf::RenderWindow* window);
		void renderSidelines(sf::RenderWindow* window);
		void renderGoals(sf::RenderWindow* window);		
		void checkIntersect(); 
		void saveIntersectingPlayers();

		void updatePhysicalObjects();

		void addPlayer();

		void movePlayer(int playerNumber, std::string direction);


	private:
		std::vector<Player> players;
		std::vector<Player> intersectingPlayers;
		sf::Image background;
		Ball ball;
};

#endif //GAME_HPP