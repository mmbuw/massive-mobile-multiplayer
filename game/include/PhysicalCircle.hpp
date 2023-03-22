#ifndef PHYSICAL_CIRCLE_HPP
#define PHYSICAL_CIRCLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <cmath>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;

#define MAX_SPEED 6.0

class PhysicalCircle
{	
	public:
		PhysicalCircle(float mass, float posX, float posY, float radius, float frictionCoefficient);
		virtual ~PhysicalCircle();
		
		virtual void render(sf::RenderWindow* window) const;
		virtual void frameFrictionUpdate(float timeFactor);
		virtual void frameUpdate();
		virtual void setPosition(float x, float y);
		virtual void clampPosition() = 0;
		virtual void setRadius(float newRadius);

		sf::CircleShape const getShape() const;
		float getPosX() const;
		float getPosY() const;
		float getVelX() const;
		float getVelY() const;
		float getMass() const;
		float getRadius() const;

		void setVelocity(float x, float y);
		void addVelocityOffset(float x, float y);
		float computeCurrentSpeed() const;

	protected:
		sf::CircleShape shape_;

		float posX_;
		float posY_;

		float velX_;
		float velY_;

		float mass_;
		float radius_;
		float frictionCoefficient_;

		Clock::time_point lastFrameTime_;
};

#endif //PHYSICAL_CIRCLE_HPP
