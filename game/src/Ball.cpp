#include "Ball.hpp"

Ball::Ball(int startX, int startY) : PhysicalObject(1.0, startX, startY, 25.0),
               inLeftGoal_(false), inRightGoal_(false),
			   startX_(startX), startY_(startY), lastPlayerTouch_(nullptr)
{
	shape_ = sf::CircleShape(radius_);
	shape_.setFillColor(sf::Color(255,255,255,255));
	shape_.setOutlineThickness(-3);
	shape_.setOutlineColor(sf::Color(0,0,0,255));
	shape_.setOrigin(radius_, radius_);

	shape_.setPosition(posX_, posY_);	

}

/* virtual */ Ball::~Ball() {}

bool Ball::handleCornerRebound(sf::Vector2f const& corner)
{
	/* source for physical corner rebounds: */
	/* http://math.stackexchange.com/questions/428546/collision-between-a-circle-and-a-rectangle */

	sf::Vector2f center(posX_, posY_);
	sf::Vector2f vecToCircle( center - corner );

	float distToCorner = std::sqrt(vecToCircle.x*vecToCircle.x + vecToCircle.y*vecToCircle.y);

	if (distToCorner < radius_)
	{
		float q = - (2*(velX_*(vecToCircle.x)+velY_*(vecToCircle.y))) / (radius_*radius_);
		velX_ = velX_ + q * vecToCircle.x;
		velY_ = velY_ + q * vecToCircle.y;
		vecToCircle = vecToCircle / distToCorner;
		setPosition(corner.x + radius_ * vecToCircle.x, corner.y + radius_ * vecToCircle.y);
		return true;
	}
	
	return false;
}


/* virtual */ void Ball::clampPosition()
{

	// if the ball was in a goal last frame, clamp upper and lower
	// border to goal lines
	if (inLeftGoal_ || inRightGoal_)
	{
		topBorderLine = goalStartHeight;
		bottomBorderLine = goalEndHeight;
	}

	/* corner clamps */
	//left goal top corner
	bool leftTop = handleCornerRebound(sf::Vector2f(leftBorderLine, goalStartHeight));

	//left goal bottom corner
	bool leftBottom = handleCornerRebound(sf::Vector2f(leftBorderLine, goalEndHeight));

	//right goal top corner
	bool rightTop = handleCornerRebound(sf::Vector2f(rightBorderLine, goalStartHeight));
	
	//right goal bottom corner
	bool rightBottom = handleCornerRebound(sf::Vector2f(rightBorderLine, goalEndHeight));

	//if a corner clamp was performed, stop checking from here on
	if (leftTop || leftBottom || rightTop || rightBottom)
	{
		return;
	}


	/* X position clamp */

	if (posX_ < leftBorderLine + radius_) // left border
	{
		// left goal
		if (posY_ >= goalStartHeight-radius_ && posY_ <= goalEndHeight+radius_)
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
		if (posY_ >= goalStartHeight-radius_ && posY_ <= goalEndHeight+radius_)
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

void Ball::resetToStart()
{
	setPosition(startX_, startY_);
	setVelocity(0, 0);
	setColor(sf::Color(255,255,255,255));
	inLeftGoal_ = false;
	inRightGoal_ = false;
	topBorderLine = temporaryTopBorder;
	bottomBorderLine = temporaryBottomBorder;
	lastPlayerTouch_ = nullptr;
}

void Ball::changeAbsorptionVelocity(bool swapX, bool swapY)
{
	float borderAbsorptionCoefficient(0.5);

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

void Ball::setLastPlayerTouch(Player* player)
{
	lastPlayerTouch_ = player;
}

std::string const Ball::getLastPlayerTouchName() const
{
	if (lastPlayerTouch_ != nullptr)
		return lastPlayerTouch_->getName(); 

	return "";
}

void Ball::setAllLines(double left, double right, double top, double bottom) {

	leftBorderLine = left;
	rightBorderLine = right;
	topBorderLine = top;
	bottomBorderLine = bottom;

	temporaryTopBorder = topBorderLine;
	temporaryBottomBorder = bottomBorderLine;

	screenWidth = sf::VideoMode::getDesktopMode().width;
	screenHeight = sf::VideoMode::getDesktopMode().height;

	double middleoflines = ((bottomBorderLine-topBorderLine)/2)+topBorderLine;

	goalStartHeight = middleoflines-(0.15*screenHeight);
	goalEndHeight = middleoflines+(0.15*screenHeight);

	leftGoalEndLine = 0.01822916*screenWidth;
	rightGoalEndLine = 0.96145832*screenWidth;

}