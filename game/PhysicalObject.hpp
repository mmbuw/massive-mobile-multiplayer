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
		
		void frameFrictionUpdate();
		void frameUpdate();
		virtual void clampPosition() = 0;

		sf::Shape const getShape() const;
		int getPosX() const;
		int getPosY() const;
		long double getVelX() const;
		long double getVelY() const;
		float getMass() const;

		void setPosition(int x, int y);

		void setVelocity(long double x, long double y);
		void addVelocityOffset(long double x, long double y);

		long double computeCurrentSpeed() const;

		void render(sf::RenderWindow* window) const;

	protected:
		sf::Shape shape_;

		int posX_;	 // pixel
		int posY_;	 // pixel

		long double velX_; // pixel per frame
		long double velY_; // pixel per frame

		float mass_; //kg
};

#endif //PHYSICAL_OBJECT_HPP