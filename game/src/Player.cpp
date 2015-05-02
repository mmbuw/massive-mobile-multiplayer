#include "Player.hpp"

Player::Player(int startX, int startY, sf::Color border, sf::Color center) :
	PhysicalObject(10.2, 0, 0, 50.0), borderColor_(border), centerColor_(center), blockShootFrames_(0), startX_(startX), startY_(startY)
{
	shootCircleRadius_ = 1.5 * radius_;

	//shape_ = sf::CircleShape(posX_, posY_, radius_, centerColor_, -5, borderColor_);
	shape_ = sf::CircleShape(radius_);
	shape_.setFillColor(centerColor_);
	shape_.setOutlineThickness(-5);
	shape_.setOutlineColor(borderColor_);

	//shootCircle_ = sf::CircleShape(posX_, posY_, shootCircleRadius_, sf::Color(0,0,0,0), -5, sf::Color(0,0,0,128));
	shootCircle_ = sf::CircleShape(shootCircleRadius_);
	shootCircle_.setPosition(posX_, posY_);
	shootCircle_.setFillColor(sf::Color(0,0,0,0));
	shootCircle_.setOutlineThickness(-5);
	shootCircle_.setOutlineColor(sf::Color(0,0,0,128));


	posX_ = startX_;
	posY_ = startY_;
	shape_.setPosition(posX_, posY_);
	shootCircle_.setPosition(posX_, posY_);
}

/* virtual */ void Player::render(sf::RenderWindow* window) const 
{
	PhysicalObject::render(window);
	window->draw(shootCircle_);
}

/* virtual */ void Player::frameUpdate()
{
	PhysicalObject::frameUpdate();

	if (inShootSequence())
	{
		--blockShootFrames_;
		if (blockShootFrames_ == 0)
		{
			
			shape_.setOutlineColor(sf::Color(0,0,0));
			
		}
	}
}

/* virtual */ void Player::setPosition(int x, int y)
{
	PhysicalObject::setPosition(x,y);
	shootCircle_.setPosition(x,y);
}

/* virtual */ void Player::clampPosition()
{
	if (posX_ < 0)
	{
		setPosition(0, posY_);
	}
	else if (posX_ > 1920)
	{
		setPosition(1920, posY_);
	}

	if (posY_ < 0)
	{
		setPosition(posX_, 0);
	}
	else if (posY_ > 1200)
	{
		setPosition(posX_, 1200);
	}
}

void Player::moveUp() 
{
	addVelocityOffset(0.0, -0.5);
}

void Player::moveDown() 
{
	addVelocityOffset(0.0, 0.5);
}

void Player::moveLeft() 
{
	addVelocityOffset(-0.5, 0.0);
}

void Player::moveRight() 
{
	addVelocityOffset(0.5, 0.0);
}

bool Player::intersectsCircle(int circlePosX, int circlePosY, float cirlceRadius, bool useShootRange) const
{
	float ownRadius;

	if (useShootRange)
		ownRadius = shootCircleRadius_;
	else
		ownRadius = radius_;

	int checkValueLower = (ownRadius - cirlceRadius) * (ownRadius - cirlceRadius);
	int checkValueUpper = (ownRadius + cirlceRadius) * (ownRadius + cirlceRadius);

	int realValue = (posX_ - circlePosX) * (posX_ - circlePosX) + 
	                (posY_ - circlePosY) * (posY_ - circlePosY);

	if (realValue <= checkValueUpper)
	{
		return true;
	}

	return false;
}

void Player::shoot()
{
	if (inShootSequence() == false)
	{
		shape_.setOutlineColor(sf::Color(255,255,255));
		blockShootFrames_ = 5;
	}
}

bool Player::inShootSequence() const
{
	if (blockShootFrames_ == 0)
		return false;

	return true;
}

void Player::resetToStart()
{
	setPosition(startX_, startY_);
	setVelocity(0.0, 0.0);
}