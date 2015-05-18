#ifndef BALL_HPP
#define BALL_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <string>


#include "PhysicalObject.hpp"

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
	
	private:
		bool inLeftGoal_;
		bool inRightGoal_;

		int startX_;
		int startY_;

		
};

#endif //BALL_HPP