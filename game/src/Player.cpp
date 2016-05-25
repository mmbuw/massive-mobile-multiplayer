#include "Player.hpp"

Player::Player(int startX, int startY, sf::Color border, sf::Color center, std::string const& name, int number) :
	PhysicalCircle(10.2, startX, startY, 35.0, 0.8), borderColor_(border), centerColor_(center), 
	blockShootFrames_(0), startX_(startX), startY_(startY), name_(name), shirtNumber_(number)
{
	shootCircleRadius_ = computeShootCircleRadius();

	setLineRestrictions();

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
	numberText_.setCharacterSize(30);

	nameText_.setFont(font_);
	nameText_.setString(name_);
	nameText_.setColor(sf::Color(0,0,0));
	nameText_.setCharacterSize(6.0/7.0 * radius_);
}


/* virtual */ void Player::render(sf::RenderWindow* window) const 
{
	PhysicalCircle::render(window);
	
	window->draw(shootCircle_);
	window->draw(nameText_);
	window->draw(numberText_);
}

/* virtual */ void Player::frameUpdate()
{
	PhysicalCircle::frameUpdate();

	nameText_.setPosition(posX_-((name_.size()/2)*20), posY_ + (50.0/35.0*radius_));

	if (shirtNumber_ > 9)
	{
		numberText_.setPosition(posX_- (4.0/7.0*radius_), posY_ - (4.0/7.0*radius_));
	}
	else
	{
		numberText_.setPosition(posX_- (8.0/35.0*radius_), posY_-(4.0/7.0*radius_));
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
	PhysicalCircle::setPosition(x,y);
	shootCircle_.setPosition(sf::Vector2f(x,y));
}

/* virtual */ void Player::clampPosition()
{
	if (posX_ < 0)
	{
		setPosition(0, posY_);
	}
	else if (posX_ > screenWidth)
	{
		setPosition(screenWidth, posY_);
	}

	if (posY_ < 0)
	{
		setPosition(posX_, 0);
	}
	else if (posY_ > screenHeight)
	{
		setPosition(posX_, screenHeight);
	}
}

/* virtual */ void Player::setRadius(float newRadius)
{
	PhysicalCircle::setRadius(newRadius);
	shootCircleRadius_ = computeShootCircleRadius();
	shootCircle_.setOrigin(0, 0);
	shootCircle_.setRadius(shootCircleRadius_);
	shootCircle_.setOrigin(shootCircleRadius_, shootCircleRadius_);
	numberText_.setCharacterSize(6.0/7.0 * radius_);
}

void Player::moveUp() 
{
	addVelocityOffset(0.0, -5.0);
}

void Player::moveDown() 
{
	addVelocityOffset(0.0, 5.0);
}

void Player::moveLeft() 
{
	addVelocityOffset(-5.0, 0.0);
}

void Player::moveRight() 
{
	addVelocityOffset(5.0, 0.0);
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

void Player::setLineRestrictions() 
{
	screenWidth = sf::VideoMode::getDesktopMode().width;
	screenHeight = sf::VideoMode::getDesktopMode().height * 0.92307692307; //because the field actually ends at the score line
}

float Player::computeShootCircleRadius() const
{
	return radius_ * 1.5;
}

sf::Color const Player::getTeamColor()
{
	return centerColor_;
}

std::string const Player::getName() const
{
	return name_;
}

int Player::getShirtNumber() const
{
	return shirtNumber_;
}
