#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <set>

#include "Player.hpp"
#include "Ball.hpp"
#include "PhysicalObject.hpp"


class Game
{
	
	public:
		Game();
		~Game();

		Player* addNewPlayer(std::string const& name, int number);
		void removePlayer(Player* playerToRemove);
	
		void renderBackground(sf::RenderWindow* window);
		void renderPlayers(sf::RenderWindow* window);		
		void renderBall(sf::RenderWindow* window);
		void renderSidelines(sf::RenderWindow* window);
		void renderGoals(sf::RenderWindow* window);
		void renderScoreLine(sf::RenderWindow* window);
		void renderFpsDisplay(sf::RenderWindow* window, float value);

		void updatePhysicalObjects();
		void applyIntersectionPhysics(); 
		void applyShootingForce(Player* player);
		void applyElasticImpact(PhysicalObject* lhs, PhysicalObject* rhs, float lhsAbsorption, float rhsAbsorption);

		void createField();

		void checkForGoal();

	private:
		std::set<Player*> players;

		std::vector<sf::ConvexShape> lines;
		std::vector<sf::RectangleShape> field;

		void createFieldLines();
		void createGreen();
		sf::CircleShape centerCircle;
		sf::CircleShape centerPoint;

		Ball* ball;

		bool ballWasInLeftGoal_;
		bool ballWasInRightGoal_;
		int framesToReset_;

		int pointsBlueTeam_;
		int pointsRedTeam_;

		int numPlayersRed_;
		int numPlayersBlue_;
};

#endif //GAME_HPP
