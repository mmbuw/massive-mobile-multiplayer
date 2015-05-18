#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <time.h>
#include <stdlib.h>

#include "Ball.hpp"
#include "PhysicalObject.hpp"


class Player : public PhysicalObject
{

	public:
		Player(int startX, int startY, sf::Color border, sf::Color center, std::string const& name, int number);
		
		/* virtual */ void render(sf::RenderWindow* window) const;
		/* virtual */ void frameUpdate();
		/* virtual */ void setPosition(float x, float y);
		/* virtual */ void clampPosition();

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		bool intersectsCircle(int cirlcePosX, int circlePosY, float cirlceRadius, bool useShootCircle) const;
		void shoot();
		bool inShootSequence() const;
		void resetToStart();
		sf::Color const getTeamColor();


	private:
		sf::Color borderColor_;
		sf::Color centerColor_;

		std::string name_;
		int shirtNumber_;

		float shootCircleRadius_;
		int blockShootFrames_;

		sf::CircleShape shootCircle_;
		int startX_;
		int startY_;

		sf::Text numberText_;
		sf::Text nameText_;
		sf::Font font_;
};

#endif //PLAYER_HPP