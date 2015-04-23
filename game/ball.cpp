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
	float borderAbsorptionCoefficient(0.7);

	if (posX_ < 105) // left border
	{
		setPosition(105, posY_);
		velX_ = -velX_;
		velX_ *= borderAbsorptionCoefficient;
		velY_ *= borderAbsorptionCoefficient;
	}
	else if (posX_ > 1820) // right border
	{
		setPosition(1820, posY_);
		velX_ = -velX_;
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