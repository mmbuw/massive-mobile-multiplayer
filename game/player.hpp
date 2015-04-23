#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "ball.hpp"
#include "PhysicalObject.hpp"

class Player : public PhysicalObject
{

	public:
		Player(int startX, int startY, sf::Color border, sf::Color center);
		
		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		bool intersectsWithBall(Ball ball);
		bool currentlyIntersectsBall;

		bool intersectsWithPlayer(Player otherGuy);
		
	private:
		sf::Color borderColor;
		sf::Color centerColor;

		void controlPosition();
		

};
