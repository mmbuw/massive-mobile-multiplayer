#include "ball.hpp"

Ball::Ball() : PhysicalObject(0.6, 0, 0)
{
	radius_ = 30.0;
	shape_ = sf::Shape::Circle(posX_, posY_, radius_, sf::Color(255,255,255), -3, sf::Color(0,0,0));

	posX_ = 960;
	posY_ = 600;
	shape_.SetPosition(posX_, posY_);
}

/* virtual */ Ball::~Ball()
{
	// do nothing
}

/* virtual */ void Ball::clampPosition()
{
	float borderAbsorptionCoefficient(0.7);

	bool inLeftGoal(false);
	bool inRightGoal(false);

	if (posX_ < 105) // left border
	{
		// left goal
		if (posY_ > 450 && posY_ < 750)
		{
			if (posX_ < 35)
			{
				setPosition(35, posY_);
				velX_ = -velX_;
				velX_ *= borderAbsorptionCoefficient;
				velY_ *= borderAbsorptionCoefficient;
			}

		}
		else
		{
			std::cout << "GO TO OUTSIDE" << std::endl;
			setPosition(105, posY_);
			velX_ = -velX_;
			velX_ *= borderAbsorptionCoefficient;
			velY_ *= borderAbsorptionCoefficient;
		}
	}
	else if (posX_ > 1820) // right border
	{
		// right goal
		if (posY_ > 450 && posY_ < 750)
		{
			if (posX_ > 1885)
			{
				setPosition(1885, posY_);
				velX_ = -velX_;
				velX_ *= borderAbsorptionCoefficient;
				velY_ *= borderAbsorptionCoefficient;
			}
		}
		else
		{
			setPosition(1820, posY_);
			velX_ = -velX_;
			velX_ *= borderAbsorptionCoefficient;
			velY_ *= borderAbsorptionCoefficient;
		}
	}

	if (posX_ < 105)
	{
		inLeftGoal = true;
		std::cout << "IN LEFT GOAL" << std::endl;
	}

	if (inLeftGoal && posY_ > 750)
	{
		std::cout << "ADJUST LARGER" << std::endl;
		setPosition(posX_, 750);
		velY_ = -velY_;
		velX_ *= borderAbsorptionCoefficient;
		velY_ *= borderAbsorptionCoefficient;
	}
	else if (inLeftGoal && posY_ < 450)
	{
		std::cout << "ADJUST SMALLER" << std::endl;
		setPosition(posX_, 450);
		velY_ = -velY_;
		velX_ *= borderAbsorptionCoefficient;
		velY_ *= borderAbsorptionCoefficient;
	}

	if (posY_ < 26) // top border
	{
		setPosition(posX_, 26);
		velY_ = -velY_;
		velX_ *= borderAbsorptionCoefficient;
		velY_ *= borderAbsorptionCoefficient;
	}
	else if(posY_ > 1150) //bottom border
	{
		setPosition(posX_, 1150);
		velY_ = -velY_;
		velX_ *= borderAbsorptionCoefficient;
		velY_ *= borderAbsorptionCoefficient;
	}
}

float Ball::getRadius() const
{
	return radius_;
}