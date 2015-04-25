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
		
		/* virtual */ void render(sf::RenderWindow* window) const;
		/* virtual */ void frameUpdate();
		/* virtual */ void setPosition(int x, int y);
		/* virtual */ void clampPosition();

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		bool intersectsCircle(int cirlcePosX, int circlePosY, float cirlceRadius, bool useShootCircle) const;
		void shoot();
		bool inShootSequence() const;
		void resetToStart();


	private:
		sf::Color borderColor_;
		sf::Color centerColor_;

		float shootCircleRadius_;
		int blockShootFrames_;

		sf::Shape shootCircle_;
		int startX_;
		int startY_;
};

#endif //PLAYER_HPP