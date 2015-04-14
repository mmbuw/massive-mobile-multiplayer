#include "ball.hpp"

Ball::Ball() {
	xPosition = 960;
	yPosition = 600;
}

void Ball::render(sf::RenderWindow* window){
	sf::Shape ballCircle = sf::Shape::Circle(xPosition,yPosition,20,sf::Color(255,255,255),-3,sf::Color(0,0,0));
	window->Draw(ballCircle);
}


