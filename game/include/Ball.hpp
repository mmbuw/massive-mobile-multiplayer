#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>

#include "PhysicalObject.hpp"
#include "Player.hpp"

class Player;

class Ball : public PhysicalObject 
{
	public:
		Ball(int startX, int startY);

		/* virtual */ ~Ball();
		/* virtual */ void clampPosition();
		
		void setColor(sf::Color color);

		void resetToStart();
		void changeAbsorptionVelocity(bool swapX, bool swapY);
		bool isInLeftGoal() const;
		bool isInRightGoal() const;
		void setLastPlayerTouch(Player* player);
		std::string const getLastPlayerTouchName() const;

		void setAllLines(double left, double right, double top, double bottom);
	
	private:
		bool inLeftGoal_;
		bool inRightGoal_;

		double screenWidth;
		double screenHeight;

		double leftBorderLine;
		double rightBorderLine;
		double temporaryTopBorder;
		double temporaryBottomBorder;
		double topBorderLine;
		double bottomBorderLine;

		double goalStartHeight;
		double goalEndHeight;


		double leftGoalEndLine;	
		double rightGoalEndLine;
	
		Player* lastPlayerTouch_;

		int startX_;
		int startY_;

		
};

#endif //BALL_HPP