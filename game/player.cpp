#include "player.hpp"

Player::Player(int startX, int startY, sf::Color border, sf::Color center) :
	PhysicalObject(10.2, 0, 0), borderColor_(border), centerColor_(center), blockShootFrames_(0), startX_(startX), startY_(startY)
{
	radius_ = 50.0;
	shootCircleRadius_ = 1.5 * radius_;

	shape_ = sf::Shape::Circle(posX_, posY_, radius_, centerColor_, -5, borderColor_);
	shootCircle_ = sf::Shape::Circle(posX_, posY_, shootCircleRadius_, sf::Color(0,0,0,0), -5, sf::Color(0,0,0,128));
	
	posX_ = startX_;
	posY_ = startY_;
	shape_.SetPosition(posX_, posY_);
	shootCircle_.SetPosition(posX_, posY_);
}

/* virtual */ void Player::render(sf::RenderWindow* window) const 
{
	PhysicalObject::render(window);
	window->Draw(shootCircle_);
}

bool Player::intersectsWithBall(Ball const& ball, bool useShootCircle) const
{
	float ownRadius;

	if (useShootCircle)
	{
		ownRadius = shootCircleRadius_;
	}
	else
	{
		ownRadius = radius_;
	}

	int checkValueLower = (ownRadius - ball.getRadius()) * (ownRadius - ball.getRadius());
	int checkValueUpper = (ownRadius + ball.getRadius()) * (ownRadius + ball.getRadius());

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

void Player::shoot()
{
	if (inShootSequence() == false)
	{
		for (int i = 0; i < shape_.GetNbPoints(); ++i)
		{
			shape_.SetPointOutlineColor(i, sf::Color(255,255,255));
		}

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

/* virtual */ void Player::frameUpdate()
{
	PhysicalObject::frameUpdate();

	if (inShootSequence())
	{
		--blockShootFrames_;
		if (blockShootFrames_ == 0)
		{
			for (int i = 0; i < shape_.GetNbPoints(); ++i)
			{
				shape_.SetPointOutlineColor(i, sf::Color(0,0,0));
			}
		}

	}

}

/* virtual */ void Player::setPosition(int x, int y)
{
	PhysicalObject::setPosition(x,y);
	shootCircle_.SetPosition(x,y);
}