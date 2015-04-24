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
		void shoot();

		/* virtual */ void clampPosition();
		float getRadius() const;
		/* virtual */ void frameUpdate();
		
	private:
		sf::Color borderColor_;
		sf::Color centerColor_;
		float radius_;

		void controlPosition();
		

};

#endif //PLAYER_HPP