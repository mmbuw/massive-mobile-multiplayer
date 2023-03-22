#include "PhysicalCircle.hpp"

PhysicalCircle::PhysicalCircle(float mass, float posX, float posY, float radius, float frictionCoefficient) : 
	mass_(mass), posX_(posX), posY_(posY), velX_(0.0), velY_(0.0), radius_(radius), frictionCoefficient_(frictionCoefficient), lastFrameTime_(Clock::now()) {}

/* virtual */ PhysicalCircle::~PhysicalCircle() {}

/* virtual */ void PhysicalCircle::render(sf::RenderWindow* window) const 
{
	window->draw(shape_);
}

/* virtual */ void PhysicalCircle::frameFrictionUpdate(float timeFactor)
{
	float frictionDecrement = frictionCoefficient_ * timeFactor;

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

/* virtual */ void PhysicalCircle::frameUpdate()
{
	Clock::time_point nowTime = Clock::now();
	int elapsedMilliseconds = (std::chrono::duration_cast<milliseconds>(nowTime - lastFrameTime_)).count();
	float timeFactor = (elapsedMilliseconds/1000.0)*100.0;
	frameFrictionUpdate(timeFactor);
	setPosition(posX_ + velX_ * timeFactor, posY_ + velY_ * timeFactor);
	lastFrameTime_ = nowTime;
}

/* virtual */ void PhysicalCircle::setPosition(float x, float y)
{
	posX_ = x;
	posY_ = y;
	shape_.setPosition(sf::Vector2f(posX_, posY_));
}

/* virtual */ void PhysicalCircle::setRadius(float newRadius)
{
	radius_ = newRadius;
	shape_.setOrigin(0, 0);
	shape_.setRadius(newRadius);
	shape_.setOrigin(newRadius, newRadius);
}


sf::CircleShape const PhysicalCircle::getShape() const
{
	return shape_;
}

float PhysicalCircle::getPosX() const
{
	return posX_;
}

float PhysicalCircle::getPosY() const
{
	return posY_;
}

float PhysicalCircle::getVelX() const
{
	return velX_;
}

float PhysicalCircle::getVelY() const
{
	return velY_;
}

float PhysicalCircle::getMass() const
{
	return mass_;
}

float PhysicalCircle::getRadius() const
{
	return radius_;
}

void PhysicalCircle::setVelocity(float x, float y)
{
	velX_ = x;
	velY_ = y;
}

void PhysicalCircle::addVelocityOffset(float x, float y)
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

float PhysicalCircle::computeCurrentSpeed() const
{
	return std::sqrt(velX_*velX_ + velY_*velY_);
}
