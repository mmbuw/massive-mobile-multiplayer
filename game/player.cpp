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
	setVelocity(0.0, -10.0);
}

void Player::moveDown() 
{
	setVelocity(0.0, 10.0);
}

void Player::moveLeft() 
{
	setVelocity(-10.0, 0.0);
}

void Player::moveRight() 
{
	setVelocity(10.0, 0.0);
}

/*
void Player::controlPosition(){
	
	if (yPosition<0){
		yPosition=0;	
	}else if(yPosition>1200){
		yPosition=1200;	
	}

	if (xPosition<0){
		xPosition=0;	
	}else if (xPosition>1920){
		xPosition=1920;	
	}
}*/