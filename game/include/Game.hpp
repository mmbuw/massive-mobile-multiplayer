#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Player.hpp"
#include "Ball.hpp"
#include "PhysicalObject.hpp"


class Game
{
	
	public:
		Game();
		~Game();

		void initPlayers();
		Player* addNewPlayer();
		void initBall();
	
		void renderBackground(sf::RenderWindow* window);
		void renderPlayers(sf::RenderWindow* window);		
		void renderBall(sf::RenderWindow* window);
		void renderSidelines(sf::RenderWindow* window);
		void renderGoals(sf::RenderWindow* window);
		void renderScoreLine(sf::RenderWindow* window);
		void renderFpsDisplay(sf::RenderWindow* window, float value);

		void updatePhysicalObjects();
		void applyIntersectionPhysics(); 
		void applyShootingForce(Player const& player);
		void applyElasticImpact(PhysicalObject& lhs, PhysicalObject& rhs, float lhsAbsorption, float rhsAbsorption);

		void checkForGoal();
		void playerShoot(int playerID);

		void movePlayer(int playerNumber, std::string direction);


	private:
		std::vector<Player*> players;
		Ball ball;

		bool ballWasInLeftGoal_;
		bool ballWasInRightGoal_;
		int framesToReset_;

		int pointsLeftTeam_;
		int pointsRightTeam_;
};

#endif //GAME_HPP
