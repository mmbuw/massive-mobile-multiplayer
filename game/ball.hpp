#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Ball {
	public:
		Ball();
		void render(sf::RenderWindow* window);
	
	private:
		int xPosition;
		int yPosition;

};
