#include "player.hpp"

Player::Player(int startX, int startY, sf::Color border, sf::Color center) :
	PhysicalObject(50.0, 0, 0)
{
	radius_ = 50.0;

	shape_ = sf::Shape::Circle(posX_, posY_, radius_, center, -5, border);
	
	posX_ = startX;
	posY_ = startY;
	shape_.SetPosition(posX_, posY_);
}


sf::Vector2f const Player::intersectsWithBall(Ball const& ball) const
{
	int checkValueLower = (radius_ - ball.getRadius()) * (radius_ - ball.getRadius());
	int checkValueUpper = (radius_ + ball.getRadius()) * (radius_ + ball.getRadius());

	int realValue = (posX_ - ball.getPosX()) * (posX_ - ball.getPosX()) + 
	                (posY_ - ball.getPosY()) * (posY_ - ball.getPosY());

	if (realValue <= checkValueUpper)
	{
		sf::Vector2f centerConnection(ball.getPosX()-posX_, ball.getPosY()-posY_);
		float factor( radius_ / (radius_ + ball.getRadius()) );
		sf::Vector2f hitPoint(posX_ + factor*centerConnection.x, posY_ + factor*centerConnection.y);
		//std::cout << "Hit point: " << hitPoint.x << " ; " << hitPoint.y << std::endl;
		return hitPoint;
	}

	return sf::Vector2f(5000, 5000);
}

bool Player::intersectsWithPlayer(Player otherGuy)
{
	/*	
	int checkValueLower = 0; // (own radius - other radius)^2 
	int checkValueUpper = 10000; // (own radius + other radius)^2 
	int realValue = (xPosition-otherGuy.getXPosition())*(xPosition-otherGuy.getXPosition()) + (yPosition-otherGuy.getYPosition())*(yPosition-otherGuy.getYPosition());
	if (realValue <= checkValueUpper){
		return true;
	}
	return false;
	*/
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