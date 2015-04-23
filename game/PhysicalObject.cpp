#include "PhysicalObject.hpp"

PhysicalObject::PhysicalObject(float mass, int posX, int posY) : 
	mass_(mass), posX_(posX), posY_(posY), velX_(0.0), velY_(0.0), accX_(0.0), accY_(0.0) {}

/* virtual */ PhysicalObject::~PhysicalObject()
{
	// do nothing
}

void PhysicalObject::frameFrictionUpdate()
{
	velX_ = std::max(0.0, velX_-1.0);
	velY_ = std::max(0.0, velY_-1.0);
}

void PhysicalObject::frameUpdate()
{
	frameFrictionUpdate();

	posX_ += velX_;
	posY_ += velY_;
	shape_.SetPosition(posX_, posY_);
	//shape_.SetX(200);
	//shape_.SetPosition(200, 200);
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

void PhysicalObject::setVelocity(float x, float y)
{
	velX_ = x;
	velY_ = y;
}