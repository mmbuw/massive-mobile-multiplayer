#ifndef PLAYER_HPP
#define PLAYER_HPP

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

		bool intersectsWithBall(Ball const& ball) const;
		bool intersectsWithPlayer(Player const& otherPlayer) const;
		bool currentlyIntersectsBall;

		/* virtual */ void clampPosition();
		float getRadius() const;
		
	private:
		sf::Color borderColor;
		sf::Color centerColor;
		float radius_;

		void controlPosition();
		

};

#endif //PLAYER_HPP