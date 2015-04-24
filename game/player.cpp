#include "player.hpp"

Player::Player(int startX, int startY, sf::Color border, sf::Color center) :
	PhysicalObject(70.0, 0, 0)
{
	radius_ = 50.0;

	shape_ = sf::Shape::Circle(posX_, posY_, radius_, center, -5, border);
	
	posX_ = startX;
	posY_ = startY;
	shape_.SetPosition(posX_, posY_);
}


bool Player::intersectsWithBall(Ball const& ball) const
{
	int checkValueLower = (radius_ - ball.getRadius()) * (radius_ - ball.getRadius());
	int checkValueUpper = (radius_ + ball.getRadius()) * (radius_ + ball.getRadius());

	int realValue = (posX_ - ball.getPosX()) * (posX_ - ball.getPosX()) + 
	                (posY_ - ball.getPosY()) * (posY_ - ball.getPosY());

	if (realValue <= checkValueUpper)
	{
		return true;
	}

	return false;
}

bool Player::intersectsWithPlayer(Player const& otherPlayer) const
{
	int checkValueLower = (radius_ - otherPlayer.getRadius()) * (radius_ - otherPlayer.getRadius());
	int checkValueUpper = (radius_ + otherPlayer.getRadius()) * (radius_ + otherPlayer.getRadius());

	int realValue = (posX_ - otherPlayer.getPosX()) * (posX_ - otherPlayer.getPosX()) + 
	                (posY_ - otherPlayer.getPosY()) * (posY_ - otherPlayer.getPosY());

	if (realValue <= checkValueUpper)
	{
		return true;
	}

	return false;
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

void Player::clampPosition()
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

float Player::getRadius() const
{
	return radius_;
}