#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Ball {
	public:
		Ball();
		void render(sf::RenderWindow* window);
		sf::Shape getShape();
		int getX();
		int getY();
		void move(int xMovement, int yMovement);	
	
	private:
		int xPosition;
		int yPosition;
		void checkPosition();
		sf::Shape shape;

};
