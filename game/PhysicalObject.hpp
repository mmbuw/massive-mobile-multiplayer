#ifndef PHYSICAL_OBJECT_HPP
#define PHYSICAL_OBJECT_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

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
		void setPosition(int x, int y);

		void setVelocity(float x, float y);
		void addVelocityOffset(float x, float y);

		void render(sf::RenderWindow* window) const;

	protected:
		sf::Shape shape_;

		int posX_;	 // pixel
		int posY_;	 // pixel

		float velX_; // pixel per frame
		float velY_; // pixel per frame

		float mass_; //kg

		float accX_;
		float accY_;
};

#endif //PHYSICAL_OBJECT_HPP