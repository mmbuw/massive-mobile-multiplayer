#include "ball.hpp"

Ball::Ball() : PhysicalObject(0.6, 0, 0)
{
	shape_ = sf::Shape::Circle(posX_, posY_, 20, sf::Color(255,255,255), -3, sf::Color(0,0,0));

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
	if (posX_ < 105)
	{
		setPosition(105, posY_);
	}
	else if (posX_ > 1820)
	{
		setPosition(1820, posY_);
	}

	if (posY_ < 26)
	{
		setPosition(posX_, 26);
	}
	else if(posY_ > 1150)
	{
		setPosition(posX_, 1150);
	}
}