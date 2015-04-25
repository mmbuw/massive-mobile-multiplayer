#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

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
	
	private:
		bool inLeftGoal_;
		bool inRightGoal_;
};

#endif //BALL_HPP