#include "ball.hpp"

Ball::Ball() {
	xPosition = 960;
	yPosition = 600;
}

void Ball::render(sf::RenderWindow* window){
	sf::Shape ballCircle = sf::Shape::Circle(xPosition,yPosition,20,sf::Color(255,255,255),-3,sf::Color(0,0,0));
	shape = ballCircle;
	window->Draw(ballCircle);
}

sf::Shape Ball::getShape(){
	return shape;
}

int Ball::getX(){
	return xPosition;
}

int Ball::getY(){
	return yPosition;
}

void Ball::move(int xMovement, int yMovement){
	xPosition+=xMovement;
	yPosition+=yMovement;
	checkPosition();
}

void Ball::checkPosition() {
	if (xPosition<105){
		xPosition=105;	
	}else if (xPosition>1820){
		xPosition=1820;	
	}

	if (yPosition<26){
		yPosition=26;	
	}else if(yPosition>957){
		yPosition=957;	
	}
}
