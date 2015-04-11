#include "game.hpp"

void Game::loadBackground(){
	sf::Image Image;
	if (!Image.LoadFromFile("field.jpg")) {
		std::cout<<"Field sprite could not load!"<<std::endl;			
	}
	background = Image;
}

void Game::renderBackground(sf::RenderWindow* window) {

		sf::Sprite Sprite;
		Sprite.SetX(0.0f);
		Sprite.SetY(0.0f);
		Sprite.SetImage(background);
		
		Sprite.Resize(Sprite.GetSize().x,Sprite.GetSize().y);
		window->Draw(Sprite);

}

void Game::initPlayers() {
	Player player1(1400,600,sf::Color(255,255,255),sf::Color(0,200,0));
	Player player2(480,600,sf::Color(0,0,0),sf::Color(255,0,0));
	players.push_back(player1);
	players.push_back(player2);		
}

void Game::renderPlayers(sf::RenderWindow* window) {
	for (int i = 0; i < players.size(); i++){
		players[i].render(window);	
	}

}

void Game::movePlayer(int playerNumber, std::string direction){

	if (direction == "UP"){
		players[playerNumber].moveUp();	
	}
	else if (direction == "DOWN"){
		players[playerNumber].moveDown();	
	}
	else if (direction == "LEFT"){
		players[playerNumber].moveLeft();	
	}
	else if (direction == "RIGHT"){
		players[playerNumber].moveRight();	
	}

}
