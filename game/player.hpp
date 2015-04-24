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
		bool inShootSequence() const;
		void resetToStart();

		/* virtual */ void clampPosition();
		float getRadius() const;
		/* virtual */ void frameUpdate();
		/* virtual */ void render(sf::RenderWindow* window) const;
		
	private:
		sf::Color borderColor_;
		sf::Color centerColor_;

		float radius_;
		int blockShootFrames_;

		sf::Shape shootCircle_;
		int startX_;
		int startY_;

		void controlPosition();
		

};

#endif //PLAYER_HPP