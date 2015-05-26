#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <set>
#include <cmath>
#include <chrono>

#include "Player.hpp"
#include "Ball.hpp"
#include "PhysicalObject.hpp"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds seconds;

#define GAME_DURATION_MINUTES 3
#define GOAL_ANIMATION_DURATION_SEC 4
#define END_ANIMATION_DURATION_SEC 4

class Game
{
	
	public:
		Game(int screenWidth, int screenHeight);
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
		void renderTime(sf::RenderWindow* window);

		void updatePhysicalObjects();
		void applyIntersectionPhysics(); 
		void applyShootingForce(Player* player);
		void applyElasticImpact(PhysicalObject* lhs, PhysicalObject* rhs, float lhsAbsorption, float rhsAbsorption, bool equal);
		
		void performEndOfGameAnimation();
		void resetScore();

		void checkForGoal();
		void resetPlayers();
		void createField();

		int getScreenWidth();
		int getScreenHeight();
		void setScreenWidth(int in);
		void setScreenHeight(int in);

	private:

		int screenWidth_;
		int screenHeight_;

		sf::Clock clock_;

		//graphical objects
		std::vector<sf::ConvexShape> lines_;
		std::vector<sf::RectangleShape> field_;
		sf::RectangleShape goalLeft_;
		sf::RectangleShape goalRight_;
		sf::CircleShape centerCircle_;
		sf::CircleShape centerPoint_;
		sf::RectangleShape scoreLine_;
	
		sf::Text blue_;
		sf::Text red_;
		sf::Text score_;

		sf::Text fpsString_;
		sf::Font font_;
		sf::Text time_;
		sf::Font scoreFont_;
		sf::CircleShape blueBox_;
		sf::CircleShape redBox_;

		sf::Text goalTextOne_;
		sf::Text goalTextTwo_;

		//internal graphical methods
		void createFieldLines();
		void createBall(int startX, int startY);
		void createGreen();
		void createGoals();
		void createScoreLine();
		void createFpsDisplay();
		void initTime();

		void calculateLinePoistions();

		double leftLineAt;
		double rightLineAt;
		double topLineAt;
		double bottomLineAt;
		double centerLineAt;

		//game logic objects
		std::set<Player*> players;

		Ball* ball;

		bool ballWasInLeftGoal_;
		bool ballWasInRightGoal_;

		Clock::time_point goalAnimationStartTime_;
		Clock::time_point endAnimationStartTime_;
		bool inGoalAnimation_;
		bool inEndAnimation_;
		sf::Color celebratingTeam_;

		int pointsBlueTeam_;
		int pointsRedTeam_;

		int numPlayersRed_;
		int numPlayersBlue_;
};

sf::Vector2f const lerp(sf::Vector2f const& start, sf::Vector2f const& end, float timeStep);
float mapTimeStep(float inputTimeStep);

#endif //GAME_HPP
