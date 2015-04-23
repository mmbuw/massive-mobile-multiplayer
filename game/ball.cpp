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

/* virtual */ void Ball::render(sf::RenderWindow* window) const
{
	window->Draw(shape_);
}

/*
void Ball::checkPosition() 
{
	if (xPosition<105)
	{
		xPosition=105;	
	}
	else if (xPosition>1820)
	{
		xPosition=1820;	
	}

	if (yPosition<26)
	{
		yPosition=26;	
	}
	else if(yPosition>957)
	{
		yPosition=957;	
	}
}*/