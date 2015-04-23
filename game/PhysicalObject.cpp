#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(float mass, int posX, int posY) : 
	mass_(mass), posX_(posX), posY_(posY), velX_(0.0), velY_(0.0), accX_(0.0), accY_(0.0) {}

/* virtual */ PhysicalObject::~PhysicalObject()
{
	// do nothing
}

void PhysicalObject::render(sf::RenderWindow* window) const 
{
	window->Draw(shape_);
}

void PhysicalObject::frameFrictionUpdate()
{
	float frictionDecrement(0.5);

	if (velX_ > 0.0)
	{
		if (velX_ - frictionDecrement < 0.0)
			velX_ = 0.0;
		else
			velX_ -= frictionDecrement;
	}
	else if (velX_ < 0.0)
	{
		if (velX_ + frictionDecrement > 0.0)
			velX_ = 0.0;
		else
			velX_ += frictionDecrement;
	}

	if (velY_ > 0.0)
	{
		if (velY_ - frictionDecrement < 0.0)
			velY_ = 0.0;
		else
			velY_ -= frictionDecrement;
	}
	else if (velY_ < 0.0)
	{
		if (velY_ + frictionDecrement > 0.0)
			velY_ = 0.0;
		else
			velY_ += frictionDecrement;
	}

	clampPosition();
}

void PhysicalObject::frameUpdate()
{
	frameFrictionUpdate();
	setPosition(posX_ + velX_, posY_ + velY_);
}

sf::Shape const PhysicalObject::getShape() const
{
	return shape_;
}

int PhysicalObject::getPosX() const
{
	return posX_;
}

int PhysicalObject::getPosY() const
{
	return posY_;
}

void PhysicalObject::setPosition(int x, int y)
{
	posX_ = x;
	posY_ = y;
	shape_.SetPosition(posX_, posY_);
}

void PhysicalObject::setVelocity(float x, float y)
{
	velX_ = x;
	velY_ = y;
}

void PhysicalObject::addVelocityOffset(float x, float y)
{
	velX_ += x;
	velY_ += y;

	float newSpeed = computeCurrentSpeed();

	if (newSpeed > MAX_SPEED)
	{
		velX_ = velX_ / newSpeed;
		velY_ = velY_ / newSpeed;
		velX_ *= MAX_SPEED;
		velY_ *= MAX_SPEED;
	}

}

float PhysicalObject::computeCurrentSpeed() const
{
	return std::sqrt(velX_*velX_ + velY_*velY_);
}