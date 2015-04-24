#include "ball.hpp"

Ball::Ball() : PhysicalObject(1.0, 0, 0), radius_(30.0), inLeftGoal_(false), inRightGoal_(false)
{
	//initialize the shape with 0 position first in order to set center properly
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

	int leftBorderLine(105);
	int rightBorderLine(1820);
	int topBorderLine(26);
	int bottomBorderLine(1180);

	int goalStartHeight(450);
	int goalEndHeight(750);

	int leftGoalEndLine(35);
	int rightGoalEndLine(1885);

	// if the ball was in a goal last frame, clamp upper and lower
	// border to goal lines
	if (inLeftGoal_ || inRightGoal_)
	{
		topBorderLine = goalStartHeight;
		bottomBorderLine = goalEndHeight;
	}


	if (posX_ < leftBorderLine + radius_) // left border
	{
		// left goal
		if (posY_ > goalStartHeight && posY_ < goalEndHeight)
		{
			if (posX_ < leftGoalEndLine + radius_)
			{
				setPosition(leftGoalEndLine + radius_, posY_);
				velX_ = -velX_;
				velX_ *= borderAbsorptionCoefficient;
				velY_ *= borderAbsorptionCoefficient;
			}

			//we want the ball's center to be over the goal line
			if (posX_ <= leftBorderLine)
				inLeftGoal_ = true;

		}
		else
		{
			setPosition(leftBorderLine + radius_, posY_);
			velX_ = -velX_;
			velX_ *= borderAbsorptionCoefficient;
			velY_ *= borderAbsorptionCoefficient;
		}
	}
	else if (posX_ > rightBorderLine - radius_) // right border
	{
		// right goal
		if (posY_ > goalStartHeight && posY_ < goalEndHeight)
		{
			if (posX_ > rightGoalEndLine - radius_)
			{
				setPosition(rightGoalEndLine - radius_, posY_);
				velX_ = -velX_;
				velX_ *= borderAbsorptionCoefficient;
				velY_ *= borderAbsorptionCoefficient;
			}

			if (posX_ >= rightBorderLine)
				inRightGoal_ = true;
		}
		else
		{
			setPosition(rightBorderLine - radius_, posY_);
			velX_ = -velX_;
			velX_ *= borderAbsorptionCoefficient;
			velY_ *= borderAbsorptionCoefficient;
		}
	}
	else
	{
		inLeftGoal_ = false;
		inRightGoal_ = false;
	}

	if (posY_ < topBorderLine + radius_) // top border
	{
		setPosition(posX_, topBorderLine + radius_);
		velY_ = -velY_;
		velX_ *= borderAbsorptionCoefficient;
		velY_ *= borderAbsorptionCoefficient;
	}
	else if(posY_ > bottomBorderLine - radius_) //bottom border
	{
		setPosition(posX_, bottomBorderLine - radius_);
		velY_ = -velY_;
		velX_ *= borderAbsorptionCoefficient;
		velY_ *= borderAbsorptionCoefficient;
	}

}

void Ball::resetToCenter()
{
	setPosition(960, 600);
	setVelocity(0, 0);
	setColor(sf::Color(255,255,255));
}

void Ball::setColor(sf::Color color)
{
	shape_.SetColor(color);
}

float Ball::getRadius() const
{
	return radius_;
}

bool Ball::isInLeftGoal() const
{
	return inLeftGoal_;
}

bool Ball::isInRightGoal() const
{
	return inRightGoal_;
}