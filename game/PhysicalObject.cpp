#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(float mass, int posX, int posY) : 
	mass_(mass), posX_(posX), posY_(posY), velX_(0.0), velY_(0.0) {}

/* virtual */ PhysicalObject::~PhysicalObject()
{
	// do nothing
}

/* virtual */ void PhysicalObject::render(sf::RenderWindow* window) const 
{
	window->Draw(shape_);
}

void PhysicalObject::frameFrictionUpdate()
{
	long double frictionDecrement(0.25);

	long double currentSpeed = computeCurrentSpeed();

	if (currentSpeed > 0.0)
	{
		long double targetSpeed;
		if (currentSpeed - frictionDecrement > 0.0)
			targetSpeed = currentSpeed-frictionDecrement;
		else
			targetSpeed = 0.0;

		velX_ = velX_ / currentSpeed;
		velY_ = velY_ / currentSpeed;
		velX_ *= targetSpeed;
		velY_ *= targetSpeed;
	}

	clampPosition();
}

/* virtual */ void PhysicalObject::frameUpdate()
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

long double PhysicalObject::getVelX() const
{
	return velX_;
}

long double PhysicalObject::getVelY() const
{
	return velY_;
}

float PhysicalObject::getMass() const
{
	return mass_;
}

void PhysicalObject::setPosition(int x, int y)
{
	posX_ = x;
	posY_ = y;
	shape_.SetPosition(posX_, posY_);
}

void PhysicalObject::setVelocity(long double x, long double y)
{
	velX_ = x;
	velY_ = y;
}

void PhysicalObject::addVelocityOffset(long double x, long double y)
{
	velX_ += x;
	velY_ += y;

	long double newSpeed = computeCurrentSpeed();

	if (newSpeed > MAX_SPEED)
	{
		velX_ = velX_ / newSpeed;
		velY_ = velY_ / newSpeed;
		velX_ *= MAX_SPEED;
		velY_ *= MAX_SPEED;
	}

}

long double PhysicalObject::computeCurrentSpeed() const
{
	return std::sqrt(velX_*velX_ + velY_*velY_);
}