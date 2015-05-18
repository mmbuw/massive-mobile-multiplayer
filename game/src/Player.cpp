#include "Player.hpp"

Player::Player(int startX, int startY, sf::Color border, sf::Color center, std::string const& name, int number) :
	PhysicalObject(10.2, startX, startY, 50.0), borderColor_(border), centerColor_(center), 
	blockShootFrames_(0), startX_(startX), startY_(startY), name_(name), shirtNumber_(number)
{
	shootCircleRadius_ = 1.5 * radius_;

	shape_ = sf::CircleShape(radius_);
	shape_.setFillColor(centerColor_);
	shape_.setOutlineThickness(-5);
	shape_.setOutlineColor(borderColor_);
	shape_.setOrigin(radius_, radius_);
	shape_.setPosition(startX, startY);

	shootCircle_ = sf::CircleShape(shootCircleRadius_);
	shootCircle_.setFillColor(sf::Color(0,0,0,0));
	shootCircle_.setOutlineThickness(-5);
	shootCircle_.setOutlineColor(sf::Color(0,0,0,255));
	shootCircle_.setOrigin(shootCircleRadius_, shootCircleRadius_);
	shootCircle_.setPosition(startX, startY);

	if (!font_.loadFromFile("resources/font.ttf"))
	{
		std::cout << "[Player.cpp] Error loading font." << std::endl;
	}

	numberText_.setFont(font_);
	numberText_.setString(std::to_string(shirtNumber_));
	numberText_.setCharacterSize(50);

	nameText_.setFont(font_);
	nameText_.setString(name_);
	nameText_.setCharacterSize(30);
}

/* virtual */ void Player::render(sf::RenderWindow* window) const 
{
	PhysicalObject::render(window);
	
	window->draw(shootCircle_);
	window->draw(nameText_);
	window->draw(numberText_);
}

/* virtual */ void Player::frameUpdate()
{
	PhysicalObject::frameUpdate();

	nameText_.setPosition(posX_-((name_.size()/2)*20), posY_+50);

	if (shirtNumber_ > 9)
	{
		numberText_.setPosition(posX_-35, posY_-35);
	}
	else
	{
		numberText_.setPosition(posX_-23, posY_-35);
	}

	if (inShootSequence())
	{
		--blockShootFrames_;
		if (blockShootFrames_ == 0)
		{
			shape_.setOutlineColor(sf::Color(0,0,0));	
		}
	}

}

/* virtual */ void Player::setPosition(float x, float y)
{
	PhysicalObject::setPosition(x,y);
	shootCircle_.setPosition(sf::Vector2f(x,y));
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

sf::Color const Player::getTeamColor()
{
	return centerColor_;
}