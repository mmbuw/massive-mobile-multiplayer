#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>

#include "PhysicalCircle.hpp"
#include "Player.hpp"

class Player;

class Ball : public PhysicalCircle 
{
	public:
		Ball(int startX, int startY);

		/* virtual */ ~Ball();
		/* virtual */ void clampPosition();
		
		bool handleCornerRebound(sf::Vector2f const& corner);
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

		double screenWidth_;
		double screenHeight_;

		double leftBorderLine_;
		double rightBorderLine_;
		double temporaryTopBorder_;
		double temporaryBottomBorder_;
		double topBorderLine_;
		double bottomBorderLine_;

		double goalStartHeight_;
		double goalEndHeight_;

		double leftGoalEndLine_;	
		double rightGoalEndLine_;
	
		Player* lastPlayerTouch_;
                sf::Texture ballTexture_;

		int startX_;
		int startY_;
};

#endif //BALL_HPP
