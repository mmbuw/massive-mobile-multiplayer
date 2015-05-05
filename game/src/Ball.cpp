#include "Ball.hpp"

Ball::Ball() : PhysicalObject(1.0, 960, 600, 30.0), inLeftGoal_(false), inRightGoal_(false)
{
	shape_ = sf::CircleShape(radius_);
	shape_.setFillColor(sf::Color(255,255,255,255));
	shape_.setOutlineThickness(-3);
	shape_.setOutlineColor(sf::Color(0,0,0,255));
	shape_.setOrigin(radius_, radius_);
	shape_.setPosition(posX_, posY_);
}

/* virtual */ Ball::~Ball() {}

/* virtual */ void Ball::clampPosition()
{
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

	/* X position clamp */

	if (posX_ < leftBorderLine + radius_) // left border
	{
		// left goal
		if (posY_ > goalStartHeight && posY_ < goalEndHeight)
		{
			if (posX_ < leftGoalEndLine + radius_)
			{
				setPosition(leftGoalEndLine + radius_, posY_);
				changeAbsorptionVelocity(true, false);
			}

			//we want the ball's center to be over the goal line
			if (posX_ <= leftBorderLine)
				inLeftGoal_ = true;

		}
		else
		{
			setPosition(leftBorderLine + radius_, posY_);
			changeAbsorptionVelocity(true, false);
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
				changeAbsorptionVelocity(true, false);
			}

			if (posX_ >= rightBorderLine)
				inRightGoal_ = true;
		}
		else
		{
			setPosition(rightBorderLine - radius_, posY_);
			changeAbsorptionVelocity(true, false);
		}
	}
	else
	{
		inLeftGoal_ = false;
		inRightGoal_ = false;
	}


	/* Y position clamp */

	if (posY_ < topBorderLine + radius_) // top border
	{
		setPosition(posX_, topBorderLine + radius_);
		changeAbsorptionVelocity(false, true);
	}
	else if(posY_ > bottomBorderLine - radius_) //bottom border
	{
		setPosition(posX_, bottomBorderLine - radius_);
		changeAbsorptionVelocity(false, true);
	}

}

void Ball::setColor(sf::Color color)
{
	shape_.setFillColor(color);
}

void Ball::resetToCenter()
{
	setPosition(960, 600);
	setVelocity(0, 0);
	setColor(sf::Color(255,255,255,255));
}

void Ball::changeAbsorptionVelocity(bool swapX, bool swapY)
{
	float borderAbsorptionCoefficient(0.7);

	if (swapX)
		velX_ = -velX_;

	if (swapY)
		velY_ = -velY_;

	velX_ *= borderAbsorptionCoefficient;
	velY_ *= borderAbsorptionCoefficient;
}

bool Ball::isInLeftGoal() const
{
	return inLeftGoal_;
}

bool Ball::isInRightGoal() const
{
	return inRightGoal_;
}