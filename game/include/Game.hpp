#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <set>
#include <cmath>
#include <chrono>
#include <array>

#include "Player.hpp"
#include "Ball.hpp"
#include "PhysicalCircle.hpp"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds seconds;

#define GAME_DURATION_MINUTES 3
#define GOAL_ANIMATION_DURATION_SEC 4
#define END_ANIMATION_DURATION_SEC 4

class Game
{
	
	public:
		Game(int screenWidth, int screenHeight, std::string resourcesPath);
		~Game();

		//player management
		Player* addNewPlayer(std::string const& name);
		void removePlayer(Player* playerToRemove);
	
		//render methods
		void renderBackground(sf::RenderWindow* window);
		void renderPlayers(sf::RenderWindow* window);		
		void renderBall(sf::RenderWindow* window);
		void renderSidelines(sf::RenderWindow* window);
		void renderGoals(sf::RenderWindow* window);
		void renderScoreLine(sf::RenderWindow* window);
		void renderFpsDisplay(sf::RenderWindow* window, float value);
		void renderTime(sf::RenderWindow* window);
		float getPlayerRadius() const;

		//physics methods
		void updatePhysicalCircles();
		void applyIntersectionPhysics(); 
		void applyShootingForce(Player* player);
		void applyElasticImpact(PhysicalCircle* lhs, PhysicalCircle* rhs, float lhsAbsorption, float rhsAbsorption, bool equal);
		
		//game logic methods
		void performEndOfGameAnimation();
		void resetScore();
		void checkForGoal();
		void resetPlayers();
		void createField();

		//getter
		int getScreenWidth();
		int getScreenHeight();

	private:

		//helpers
		int screenWidth_;
		int screenHeight_;
		sf::Clock clock_;
		bool goalAnimationScoreUpdateDone_;
		std::array<bool, 99> blueNumbersInUse_;
		std::array<bool, 99> redNumbersInUse_;

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
		void calculateLinePoistions();
		void initTime();

		//line locations
		double leftLineAt_;
		double rightLineAt_;
		double topLineAt_;
		double bottomLineAt_;
		double centerLineAt_;

		//game logic objects
		std::set<Player*> players_;
		Ball* ball_;
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

		std::string resourcesPath_;
};

sf::Vector2f const lerp(sf::Vector2f const& start, sf::Vector2f const& end, float timeStep);
float mapTimeStep(float inputTimeStep);

#endif //GAME_HPP
