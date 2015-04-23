#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "PhysicalObject.hpp"

class Ball : public PhysicalObject {
	public:
		Ball();
		/* virtual */ ~Ball();
		
		/* virtual */ void render(sf::RenderWindow* window) const;
	
	private:
		void checkPosition();

};
