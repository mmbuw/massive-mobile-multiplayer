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
		void renderGoals(sf::RenderWindow* window);		
		void checkIntersect(); 
		void saveIntersectingPlayers();

		void addPlayer();

		void movePlayer(int playerNumber, std::string direction);
		void getBallMovement();
		void resetBallMovement();
		void moveBall();
		Game();


	private:
		std::vector<Player> players;
		std::vector<Player> intersectingPlayers;
		sf::Image background;
		Ball ball;
		int ballXMovement;
		int ballYMovement;

};
