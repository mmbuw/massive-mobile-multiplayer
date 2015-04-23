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
		float getRadius() const;
		void setColor(sf::Color color);

		void resetToCenter();

		bool isInLeftGoal() const;
		bool isInRightGoal() const;
	
	private:
		void checkPosition();
		float radius_;

		bool inLeftGoal_;
		bool inRightGoal_;
};

#endif //BALL_HPP