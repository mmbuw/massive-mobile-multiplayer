#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define MAX_SPEED 6.0

class PhysicalObject
{	
	public:
		PhysicalObject(float mass, float posX, float posY, float radius);
		virtual ~PhysicalObject();
		
		virtual void render(sf::RenderWindow* window) const;
		virtual void frameFrictionUpdate();
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
		float computeCurrentSpeed() const;

		void setVelocity(float x, float y);
		void addVelocityOffset(float x, float y);

	protected:
		sf::CircleShape shape_;

		float posX_;
		float posY_;

		float velX_;
		float velY_;

		float mass_;
		float radius_;
};

#endif //PHYSICAL_OBJECT_HPP