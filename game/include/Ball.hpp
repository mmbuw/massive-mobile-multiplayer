#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>


#include "PhysicalObject.hpp"

class Ball : public PhysicalObject 
{
	public:
		Ball();

		/* virtual */ ~Ball();
		/* virtual */ void clampPosition();
		
		void setColor(sf::Color color);

		void resetToCenter();
		void changeAbsorptionVelocity(bool swapX, bool swapY);
		bool isInLeftGoal() const;
		bool isInRightGoal() const;

		void setAllLines(double left, double right, double top, double bottom);
	
	private:
		bool inLeftGoal_;
		bool inRightGoal_;

		double screenWidth;
		double screenHeight;

		double leftLineAt;
		double rightLineAt;
		double topLineAt;
		double bottomLineAt;
		double backNetLeft;
		double backNetRight;
		double goalStartHeight;
		double goalEndHeight;

		double leftGoalEndLine;	
		double rightGoalEndLine;
	


};

#endif //BALL_HPP