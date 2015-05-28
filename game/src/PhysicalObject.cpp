#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(float mass, float posX, float posY, float radius) : 
	mass_(mass), posX_(posX), posY_(posY), velX_(0.0), velY_(0.0), radius_(radius) {}

/* virtual */ PhysicalObject::~PhysicalObject() {}

/* virtual */ void PhysicalObject::render(sf::RenderWindow* window) const 
{
	window->draw(shape_);
}

/* virtual */ void PhysicalObject::frameFrictionUpdate()
{
	float frictionDecrement(0.25);

	float currentSpeed = computeCurrentSpeed();

	if (currentSpeed > 0.0)
	{
		float targetSpeed;
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

/* virtual */ void PhysicalObject::setPosition(float x, float y)
{
	posX_ = x;
	posY_ = y;
	shape_.setPosition(sf::Vector2f(posX_, posY_));
}

/* virtual */ void PhysicalObject::setRadius(float newRadius)
{
	radius_ = newRadius;
	shape_.setOrigin(0, 0);
	shape_.setRadius(newRadius);
	shape_.setOrigin(newRadius, newRadius);
}


sf::CircleShape const PhysicalObject::getShape() const
{
	return shape_;
}

float PhysicalObject::getPosX() const
{
	return posX_;
}

float PhysicalObject::getPosY() const
{
	return posY_;
}

float PhysicalObject::getVelX() const
{
	return velX_;
}

float PhysicalObject::getVelY() const
{
	return velY_;
}

float PhysicalObject::getMass() const
{
	return mass_;
}

float PhysicalObject::getRadius() const
{
	return radius_;
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