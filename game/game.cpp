#include "game.hpp"

Game::Game() : ballWasInLeftGoal_(false), ballWasInRightGoal_(false), 
			   framesToReset_(-1), pointsLeftTeam_(0), pointsRightTeam_(0) 
{
	initPlayers();
	initBall();
}

void Game::initPlayers() 
{
	Player player2(480,600,sf::Color(0,0,0),sf::Color(0,0,255));	
	Player player1(1400,600,sf::Color(0,0,0),sf::Color(255,0,0));

	players.push_back(player2);
	players.push_back(player1);			
}

void Game::initBall() 
{
	ball = Ball();
}

void Game::renderBackground(sf::RenderWindow* window) 
{
	int pos = 0;

	for (int i = 0; i < 10; ++i)
	{
		sf::Color currentGreen;	

		if (i%2 == 0)
		{
			currentGreen = sf::Color(50,200,50);						
		}
		else
		{
			currentGreen = sf::Color(75,220,75);
		}

		sf::Shape rect = sf::Shape::Rectangle(pos,0,pos+192,1200,currentGreen);
		window->Draw(rect);
		pos+=192;
	}
}

void Game::renderPlayers(sf::RenderWindow* window) 
{
	for (int i = 0; i < players.size(); i++)
	{
		players[i].render(window);	
	}
}

void Game::renderBall(sf::RenderWindow* window)
{
	ball.render(window);
}

void Game::renderSidelines(sf::RenderWindow* window) 
{
	sf::Shape leftGoalLine;
	leftGoalLine.AddPoint(100, 26, sf::Color(255, 255, 255), sf::Color(255, 255, 255));
	leftGoalLine.AddPoint(100, 1173, sf::Color(255, 255, 255), sf::Color(22, 255, 255));
	leftGoalLine.AddPoint(105, 1173, sf::Color(255, 255, 255), sf::Color(22, 255, 255));
	leftGoalLine.AddPoint(105, 26, sf::Color(255, 255, 255), sf::Color(22, 255, 255));
	window->Draw(leftGoalLine);

	sf::Shape topSideLine;
	topSideLine.AddPoint(100, 26,  sf::Color(255, 255, 255),     sf::Color(255, 255, 255));
	topSideLine.AddPoint(1820, 26,  sf::Color(255, 255, 255),     sf::Color(255, 255, 255));
	topSideLine.AddPoint(1820, 31,  sf::Color(255, 255, 255),     sf::Color(255, 255, 255));
	topSideLine.AddPoint(105, 31,   sf::Color(255, 255, 255),   sf::Color(22, 255, 255));	
	window->Draw(topSideLine);

	sf::Shape rightGoalLine;
	rightGoalLine.AddPoint(1820,26, sf::Color(255,255,255), sf::Color(255,255,255));
	rightGoalLine.AddPoint(1820,1178, sf::Color(255,255,255), sf::Color(255,255,255));
	rightGoalLine.AddPoint(1815,1178, sf::Color(255,255,255), sf::Color(255,255,255));
	rightGoalLine.AddPoint(1815,26, sf::Color(255,255,255), sf::Color(255,255,255));
	window->Draw(rightGoalLine);

	sf::Shape bottomSideLine;
	bottomSideLine.AddPoint(1820,1178,sf::Color(255,255,255),sf::Color(255,255,255));
	bottomSideLine.AddPoint(100,1178,sf::Color(255,255,255),sf::Color(255,255,255));
	bottomSideLine.AddPoint(100,1173,sf::Color(255,255,255),sf::Color(255,255,255));
	bottomSideLine.AddPoint(1820,1173,sf::Color(255,255,255),sf::Color(255,255,255));
	window->Draw(bottomSideLine);

	sf::Shape centerLine;
	centerLine.AddPoint(957,26,sf::Color(255,255,255),sf::Color(255,255,255));
	centerLine.AddPoint(957,1178,sf::Color(255,255,255),sf::Color(255,255,255));
	centerLine.AddPoint(962,1178,sf::Color(255,255,255),sf::Color(255,255,255));
	centerLine.AddPoint(962,26,sf::Color(255,255,255),sf::Color(255,255,255));
	window->Draw(centerLine);
	
	sf::Shape kickoffCircle = sf::Shape::Circle(960,600,185,sf::Color(0,0,0,0),5.f,sf::Color(255,255,255));
	window->Draw(kickoffCircle);

	sf::Shape kickoffPoint = sf::Shape::Circle(960,600,10,sf::Color(255,255,255));
	window->Draw(kickoffPoint);
}

void Game::renderGoals(sf::RenderWindow* window)
{
	sf::Shape goalLeft = sf::Shape::Rectangle(35,450,100,750,sf::Color(0,0,0,0),5,sf::Color(0,0,0));
	sf::Shape goalRight = sf::Shape::Rectangle(1820,450,1885,750,sf::Color(0,0,0,0),5,sf::Color(0,0,0));
	window->Draw(goalLeft);
	window->Draw(goalRight);
}

void Game::renderScoreLine(sf::RenderWindow* window) 
{
	sf::Shape scoreLine = sf::Shape::Rectangle(0,1200,1920,1350,sf::Color(0,0,0));
	sf::String score;
	score.SetFont(sf::Font::GetDefaultFont());
	score.SetText("Blue   "+std::to_string(pointsLeftTeam_)+":"+std::to_string(pointsRightTeam_)+"   Red");
	score.SetSize(80);
	score.Move(685.f,1200.f);

	sf::Shape blueBox = sf::Shape::Circle(600,1250,35,sf::Color(0,0,255));
	sf::Shape redBox = sf::Shape::Circle(1360,1250,35,sf::Color(255,0,0));

	window->Draw(scoreLine);
	window->Draw(blueBox);
	window->Draw(redBox);
	window->Draw(score);

}

void Game::updatePhysicalObjects()
{
	// update ball
	ball.frameUpdate();

	// update players
	for (int i = 0; i < players.size(); i++)
	{
		players[i].frameUpdate();	
	}
}

void Game::applyShootingForce(Player const& player)
{
	// apply shooting forces
	if (player.intersectsCircle(ball.getPosX(), ball.getPosY(), ball.getRadius(), true) && player.inShootSequence())
	{
		sf::Vector2f shootDir( ball.getPosX() - player.getPosX(), ball.getPosY() - player.getPosY() );
		float scaleFactor(0.2);
		ball.setVelocity(scaleFactor * shootDir.x, scaleFactor * shootDir.y);
	}
}

void Game::applyIntersectionPhysics()
{
	// for each player
	for (int i = 0; i < players.size(); ++i)
	{
		applyShootingForce(players[i]);

		// if not shooting, check for an intersection of each player with the ball
		if (players[i].intersectsCircle(ball.getPosX(), ball.getPosY(), ball.getRadius(), false))
		{
			//elastic impact computation
			float ballMass = ball.getMass();
			float playerMass = players[i].getMass();

			sf::Vector2f ballVel(ball.getVelX(), ball.getVelY());
			sf::Vector2f playerVel(players[i].getVelX(), players[i].getVelY());

			sf::Vector2f ballVelAfterCollision( (ballMass * ballVel.x + playerMass * (2 * playerVel.x - ballVel.x)) / (ballMass + playerMass),
												(ballMass * ballVel.y + playerMass * (2 * playerVel.y - ballVel.y)) / (ballMass + playerMass));

			sf::Vector2f playerVelAfterCollision( (playerMass * playerVel.x + ballMass * (2 * ballVel.x - playerVel.x)) / (ballMass + playerMass),
			                                      (playerMass * playerVel.y + ballMass * (2 * ballVel.y - playerVel.y)) / (ballMass + playerMass));
			

			//scale reflection with speed of player
			float ballPlayerReflectionFactor(0.1 * players[i].computeCurrentSpeed());
			ball.setVelocity(ballPlayerReflectionFactor * ballVelAfterCollision.x, ballPlayerReflectionFactor * ballVelAfterCollision.y);
			players[i].setVelocity(playerVelAfterCollision.x, playerVelAfterCollision.y);

			//pull player and ball apart such that they can not overlap
			sf::Vector2f ballCenter(ball.getPosX(), ball.getPosY());
			sf::Vector2f playerCenter(players[i].getPosX(), players[i].getPosY());
			float ballRadius(ball.getRadius());
			float playerRadius(players[i].getRadius());

			sf::Vector2f diffVec = playerCenter - ballCenter;
			float diffLength = std::sqrt(diffVec.x*diffVec.x + diffVec.y*diffVec.y);

			if (diffLength < ballRadius + playerRadius)
			{
				float incrementAtEachVectorEnd( (ballRadius+playerRadius-diffLength) / 2.0 );
				diffVec = diffVec / diffLength;
				diffVec = diffVec * (incrementAtEachVectorEnd);

				sf::Vector2f newBallPosition(ballCenter - diffVec);
				sf::Vector2f newPlayerPosition(playerCenter + diffVec);

				ball.setPosition(newBallPosition.x, newBallPosition.y);
				players[i].setPosition(newPlayerPosition.x, newPlayerPosition.y);
			}
		}

		// check for intersections with other players
		for (int j = 0; j < i; ++j)
		{
			// an intersection with the other player was found
			if (players[i].intersectsCircle(players[j].getPosX(), players[j].getPosY(), players[j].getRadius(), false))
			{
				//elastic impact computation
				float playerIMass = players[i].getMass();
				float playerJMass = players[j].getMass();

				sf::Vector2f playerIVel(players[i].getVelX(), players[i].getVelY());
				sf::Vector2f playerJVel(players[j].getVelX(), players[j].getVelY());

				sf::Vector2f playerIVelAfterCollision( (playerIMass * playerIVel.x + playerJMass * (2 * playerJVel.x - playerIVel.x)) / (playerIMass + playerJMass),
													   (playerIMass * playerIVel.y + playerJMass * (2 * playerJVel.y - playerIVel.y)) / (playerIMass + playerJMass));

				sf::Vector2f playerJVelAfterCollision( (playerJMass * playerJVel.x + playerIMass * (2 * playerIVel.x - playerJVel.x)) / (playerIMass + playerJMass),
				                                       (playerJMass * playerJVel.y + playerIMass * (2 * playerIVel.y - playerJVel.y)) / (playerIMass + playerJMass));
				
				players[i].setVelocity(playerIVelAfterCollision.x, playerIVelAfterCollision.y);
				players[j].setVelocity(playerJVelAfterCollision.x, playerJVelAfterCollision.y);

				//pull player and ball apart such that they can not overlap
				sf::Vector2f playerICenter(players[i].getPosX(), players[i].getPosY());
				sf::Vector2f playerJCenter(players[j].getPosX(), players[j].getPosY());
				float playerIRadius(players[i].getRadius());
				float playerJRadius(players[j].getRadius());

				sf::Vector2f diffVec = playerJCenter - playerICenter;
				float diffLength = std::sqrt(diffVec.x*diffVec.x + diffVec.y*diffVec.y);

				if (diffLength < playerIRadius + playerJRadius)
				{
					float incrementAtEachVectorEnd( (playerIRadius+playerJRadius-diffLength) / 2.0 );
					diffVec = diffVec / diffLength;
					diffVec = diffVec * (incrementAtEachVectorEnd);

					sf::Vector2f newPlayerIPosition(playerICenter - diffVec);
					sf::Vector2f newPlayerJPosition(playerJCenter + diffVec);

					players[i].setPosition(newPlayerIPosition.x, newPlayerIPosition.y);
					players[j].setPosition(newPlayerJPosition.x, newPlayerJPosition.y);
				}
			}
		}
	}
}

void Game::checkForGoal()
{
	if (ball.isInLeftGoal() && ballWasInLeftGoal_ == false && framesToReset_ == -1)
	{
		++pointsRightTeam_;
		ballWasInLeftGoal_ = true;
		framesToReset_ = 100;
		ball.setColor(sf::Color(0,0,0));

		std::cout << std::endl;
		std::cout << "The left team scored a goal!" << std::endl;
		std::cout << "Current standings: Left " << pointsLeftTeam_ << " : " << pointsRightTeam_ << " Right" << std::endl;
		std::cout << std::endl;
	}
	else if (ball.isInRightGoal() && ballWasInRightGoal_ == false && framesToReset_ == -1)
	{
		++pointsLeftTeam_;
		ballWasInRightGoal_ = true;
		framesToReset_ = 100;
		ball.setColor(sf::Color(0,0,0));

		std::cout << std::endl;
		std::cout << "The left team scored a goal!" << std::endl;
		std::cout << "Current standings: Left " << pointsLeftTeam_ << " : " << pointsRightTeam_ << " Right" << std::endl;
		std::cout << std::endl;
	}
	else if (ball.isInLeftGoal() == false && ballWasInLeftGoal_ == true)
	{
		ballWasInLeftGoal_ = false;
	}
	else if (ball.isInRightGoal() == false && ballWasInRightGoal_ == true)
	{
		ballWasInRightGoal_ = false;
	}

	// count frames until ball is reset (after scoring animation)
	if (framesToReset_ > -1)
	{
		if (framesToReset_ == 0)
		{
			ball.resetToCenter();
			
			for (int i = 0; i < players.size(); ++i)
			{
				players[i].resetToStart();
			}

			framesToReset_ = -1;
		}
		else
		{
			ball.setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
			--framesToReset_;
		}
	}
}

void Game::playerShoot(int playerID)
{
	players[playerID].shoot();
}

void Game::movePlayer(int playerNumber, std::string direction)
{
	if (direction == "UP")
	{
		players[playerNumber].moveUp();	
	}
	else if (direction == "DOWN")
	{
		players[playerNumber].moveDown();	
	}
	else if (direction == "LEFT")
	{
		players[playerNumber].moveLeft();	
	}
	else if (direction == "RIGHT")
	{
		players[playerNumber].moveRight();	
	}
}