#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "player.hpp"


class Game {
	
	public: 
		void renderBackground(sf::RenderWindow* window);
		void renderPlayers(sf::RenderWindow* window);
		void initPlayers();
		void addPlayer();
		void movePlayer(int playerNumber, std::string direction);
		void loadBackground();

	private:
		std::vector<Player> players;
		sf::Image background;	

};
