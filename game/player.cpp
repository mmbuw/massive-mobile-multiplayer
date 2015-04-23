#include "player.hpp"

Player::Player(int startX, int startY, sf::Color border, sf::Color center) :
	PhysicalObject(50.0, 0, 0)
{
	shape_ = sf::Shape::Circle(posX_, posY_, 45, center, 5, border);
	
	posX_ = startX;
	posY_ = startY;
	shape_.SetPosition(posX_, posY_);
}


bool Player::intersectsWithBall(Ball ball)
{
	/*

		//player radius 50
		//ball radius 20

	int checkValueLower = 900;
	int checkValueUpper = 4900;

	int realValue = (xPosition-ball.getPosX())*(xPosition-ball.getPosX()) + (yPosition-ball.getPosY())*(yPosition-ball.getPosY());
	if (realValue <= checkValueUpper){
		
			currentlyIntersectsBall = true;
			return true;		
	
	}
	currentlyIntersectsBall = false;
	return false;*/

}

bool Player::intersectsWithPlayer(Player otherGuy)
{
	/*
	int checkValueLower = 0;
	int checkValueUpper = 10000;
	int realValue = (xPosition-otherGuy.getXPosition())*(xPosition-otherGuy.getXPosition()) + (yPosition-otherGuy.getYPosition())*(yPosition-otherGuy.getYPosition());
	if (realValue <= checkValueUpper){
		return true;
	}
	return false;*/
	
}

void Player::moveUp() 
{
	addVelocityOffset(0.0, -1.0);
}

void Player::moveDown() 
{
	addVelocityOffset(0.0, 1.0);
}

void Player::moveLeft() 
{
	addVelocityOffset(-1.0, 0.0);
}

void Player::moveRight() 
{
	addVelocityOffset(1.0, 0.0);
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