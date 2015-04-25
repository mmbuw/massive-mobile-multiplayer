#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define MAX_SPEED 10.0

class PhysicalObject
{	
	public:
		PhysicalObject(float mass, int posX, int posY);
		virtual ~PhysicalObject();
		
		virtual void render(sf::RenderWindow* window) const;
		virtual void frameFrictionUpdate();
		virtual void frameUpdate();
		virtual void setPosition(int x, int y);
		virtual void clampPosition() = 0;

		sf::Shape const getShape() const;
		int getPosX() const;
		int getPosY() const;
		double getVelX() const;
		double getVelY() const;
		float getMass() const;
		double computeCurrentSpeed() const;

		void setVelocity(double x, double y);
		void addVelocityOffset(double x, double y);

	protected:
		sf::Shape shape_;

		int posX_;
		int posY_;

		double velX_;
		double velY_;

		float mass_;
};

#endif //PHYSICAL_OBJECT_HPP