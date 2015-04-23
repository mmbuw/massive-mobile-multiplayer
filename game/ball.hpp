#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "PhysicalObject.hpp"

class Ball : public PhysicalObject {
	public:
		Ball();
		/* virtual */ ~Ball();

		/* virtual */ void clampPosition();
	
	private:
		void checkPosition();

};
