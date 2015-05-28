#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <time.h>
#include <stdlib.h>

#include "Ball.hpp"
#include "PhysicalCircle.hpp"


class Player : public PhysicalCircle
{

	public:
		Player(int startX, int startY, sf::Color border, sf::Color center, std::string const& name, int number);
		
		/* virtual */ void render(sf::RenderWindow* window) const;
		/* virtual */ void frameUpdate();
		/* virtual */ void setPosition(float x, float y);
		/* virtual */ void clampPosition();
		/* virtual */ void setRadius(float newRadius);

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		bool intersectsCircle(int cirlcePosX, int circlePosY, float cirlceRadius, bool useShootCircle) const;
		void shoot();
		bool inShootSequence() const;
		void resetToStart();
		void setLineRestrictions();
		float computeShootCircleRadius() const;
		sf::Color const getTeamColor();
		std::string const getName() const;


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

		int screenWidth;
		int screenHeight;
		
};

#endif //PLAYER_HPP