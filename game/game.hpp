#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

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
		void renderScoreLine(sf::RenderWindow* window);		
		void checkIntersect(); 
		void saveIntersectingPlayers();

		void updatePhysicalObjects();
		void checkForGoal();
		void playerShoot(int playerID);

		void addPlayer();
		void movePlayer(int playerNumber, std::string direction);


	private:
		std::vector<Player> players;
		std::vector<Player> intersectingPlayers;
		sf::Image background;
		Ball ball;

		bool ballWasInLeftGoal_;
		bool ballWasInRightGoal_;
		int framesToResetBall_;

		int pointsLeftTeam_;
		int pointsRightTeam_;
};

#endif //GAME_HPP
