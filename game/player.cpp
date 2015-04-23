#include "player.hpp"

 Player::Player(int StartX, int StartY, sf::Color border, sf::Color center){
	xPosition = StartX;
	yPosition = StartY;
	borderColor = border;
	centerColor = center;
	currentlyIntersectsBall=false;
	xBallMovement=0;
	yBallMovement=0;
}

 Player::Player() {
	xPosition = 0;
	yPosition = 0;
	borderColor = sf::Color(0,0,0);
	centerColor = sf::Color(255,0,0);
	currentlyIntersectsBall=false;
	xBallMovement=0;
	yBallMovement=0;
}

void Player::render(sf::RenderWindow* renderWindow) {

		sf::Shape playerdot = sf::Shape::Circle(xPosition,yPosition,45,centerColor,5,borderColor);
		shape = playerdot;
		renderWindow->Draw(playerdot);
}

sf::Shape Player::getShape(){
	return shape;
}

bool Player::intersectsWithBall(Ball ball){

		//player radius 50
		//ball radius 20

	int checkValueLower = 900;
	int checkValueUpper = 4900;

	int realValue = (xPosition-ball.getX())*(xPosition-ball.getX()) + (yPosition-ball.getY())*(yPosition-ball.getY());
	if (realValue <= checkValueUpper){
		
			currentlyIntersectsBall = true;
			return true;		
	
	}
	currentlyIntersectsBall = false;
	return false;

}

bool Player::intersectsWithPlayer(Player otherGuy){
	int checkValueLower = 0;
	int checkValueUpper = 10000;
	int realValue = (xPosition-otherGuy.getXPosition())*(xPosition-otherGuy.getXPosition()) + (yPosition-otherGuy.getYPosition())*(yPosition-otherGuy.getYPosition());
	if (realValue <= checkValueUpper){
		return true;
	}
	return false;
	
	

}

int Player::getYPosition(){
	return yPosition;
}

int Player::getXPosition(){
	return xPosition;
}

void Player::moveUp() {
	yPosition-=5;
	controlPosition();
	if (currentlyIntersectsBall){
		yBallMovement-=5;
	}

}

void Player::moveDown() {
	yPosition+=5;
	controlPosition();
	if (currentlyIntersectsBall){
		yBallMovement+=5;
	}
}

void Player::moveLeft() {
	xPosition-=5;
	controlPosition();
	if (currentlyIntersectsBall){
		xBallMovement-=5;
	}
}

void Player::moveRight() {
	xPosition+=5;
	controlPosition();
	if (currentlyIntersectsBall){
		xBallMovement+=5;
	}
}

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
}

void Player::resetBallMovement() {
	xBallMovement=0;
	yBallMovement=0;	
}

int Player::getXBallMovement() {
	return xBallMovement;
}

int Player::getYBallMovement() {
	return yBallMovement;
}

