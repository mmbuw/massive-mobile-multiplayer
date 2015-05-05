#include "Game.hpp"

Game::Game() : ballWasInLeftGoal_(false), ballWasInRightGoal_(false), 
			   framesToReset_(-1), pointsLeftTeam_(0), pointsRightTeam_(0) 
{
	//initPlayers();
	initBall();
}

Game::~Game()
{
	for (std::vector<Player*>::iterator it = players.begin(); it != players.end(); ++it)
	{
		delete (*it);
	}
}

void Game::initPlayers() 
{
	//Player* player2 = new Player(480,600,sf::Color(0,0,0),sf::Color(0,0,255));	
	//Player* player1 = new Player(1400,600,sf::Color(0,0,0),sf::Color(255,0,0));

	//players.push_back(player2);
	//players.push_back(player1);
}

Player* Game::addNewPlayer()
{
	Player* newPlayer = new Player(480,600,sf::Color(0,0,0),sf::Color(0,0,255));
	players.push_back(newPlayer);
	return newPlayer;
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

		sf::RectangleShape rect(sf::Vector2f(pos+192, 1200));
		rect.setFillColor(currentGreen);
		rect.setPosition(pos, 0);
		window->draw(rect);
		pos+=192;
	}
}

void Game::renderPlayers(sf::RenderWindow* window) 
{
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->render(window);	
	}
}

void Game::renderBall(sf::RenderWindow* window)
{
	ball.render(window);
}

void Game::renderSidelines(sf::RenderWindow* window) 
{
	sf::ConvexShape leftGoalLine;
	leftGoalLine.setPointCount(4);
	leftGoalLine.setPoint(0, sf::Vector2f(100, 26));
	leftGoalLine.setPoint(1, sf::Vector2f(100, 1173));
	leftGoalLine.setPoint(2, sf::Vector2f(105, 1173));
	leftGoalLine.setPoint(3, sf::Vector2f(105, 26));
	window->draw(leftGoalLine);

	sf::ConvexShape rightGoalLine;
	rightGoalLine.setPointCount(4);
	rightGoalLine.setPoint(0, sf::Vector2f(1815, 26));
	rightGoalLine.setPoint(1, sf::Vector2f(1815, 1173));
	rightGoalLine.setPoint(2, sf::Vector2f(1820, 1173));
	rightGoalLine.setPoint(3, sf::Vector2f(1820, 26));
	window->draw(rightGoalLine);

	sf::ConvexShape topSideLine;
	topSideLine.setPointCount(4);
	topSideLine.setPoint(0, sf::Vector2f(100, 26));
	topSideLine.setPoint(1, sf::Vector2f(1820, 26));
	topSideLine.setPoint(2, sf::Vector2f(1820, 31));
	topSideLine.setPoint(3, sf::Vector2f(105, 31));
	window->draw(topSideLine);

	sf::ConvexShape bottomSideLine;
	bottomSideLine.setPointCount(4);
	bottomSideLine.setPoint(0, sf::Vector2f(1820, 1178));
	bottomSideLine.setPoint(1, sf::Vector2f(100, 1178));
	bottomSideLine.setPoint(2, sf::Vector2f(100, 1173));
	bottomSideLine.setPoint(3, sf::Vector2f(1820, 1173));
	window->draw(bottomSideLine);

	sf::ConvexShape centerLine;
	centerLine.setPointCount(4);
	centerLine.setPoint(0, sf::Vector2f(957, 26));
	centerLine.setPoint(1, sf::Vector2f(957, 1178));
	centerLine.setPoint(2, sf::Vector2f(962, 1178));
	centerLine.setPoint(3, sf::Vector2f(962, 26));
	window->draw(centerLine);

	sf::CircleShape kickoffCircle = sf::CircleShape(185);
	kickoffCircle.setFillColor(sf::Color(255,255,255,0));
	kickoffCircle.setOutlineThickness(5);
	kickoffCircle.setOutlineColor(sf::Color(255,255,255));
	kickoffCircle.setOrigin(kickoffCircle.getRadius(), kickoffCircle.getRadius());
	kickoffCircle.setPosition(960, 600);
	window->draw(kickoffCircle);

	sf::CircleShape kickoffPoint = sf::CircleShape(10);
	kickoffPoint.setFillColor(sf::Color(255,255,255));
	kickoffPoint.setOutlineColor(sf::Color(255,255,255));
	kickoffPoint.setOrigin(kickoffPoint.getRadius(), kickoffPoint.getRadius());
	kickoffPoint.setPosition(960, 600);
	window->draw(kickoffPoint);
}

void Game::renderGoals(sf::RenderWindow* window)
{
	//sf::RectangleShape goalLeft = sf::RectangleShape(35,450,100,750,sf::Color(0,0,0,0),5,sf::Color(0,0,0));
	sf::RectangleShape goalLeft = sf::RectangleShape(sf::Vector2f(65,300));
	goalLeft.setPosition(35,450);
	goalLeft.setFillColor(sf::Color(0,0,0,0));
	goalLeft.setOutlineThickness(5);
	goalLeft.setOutlineColor(sf::Color(255,255,255));
	//sf::RectangleShape goalRight = sf::RectangleShape(1820,450,1885,750,sf::Color(0,0,0,0),5,sf::Color(0,0,0));
	sf::RectangleShape goalRight = sf::RectangleShape(sf::Vector2f(65,300));
	goalRight.setPosition(1820,450);
	goalRight.setFillColor(sf::Color(0,0,0,0));
	goalRight.setOutlineThickness(5);
	goalRight.setOutlineColor(sf::Color(255,255,255));

	window->draw(goalLeft);
	window->draw(goalRight);
}

void Game::renderScoreLine(sf::RenderWindow* window) 
{
	//sf::RectangleShape scoreLine = sf::RectangleShape(0,1200,1920,1350,sf::Color(0,0,0));
	sf::RectangleShape scoreLine = sf::RectangleShape(sf::Vector2f(1920,150));
	scoreLine.setPosition(0,1200);
	sf::Text score;
	sf::Text fpsString;
	sf::Font font;
	if (!font.loadFromFile("font.ttf")){
		std::cout<<"The sadness will last forever."<<std::endl;
	}
	score.setFont(font);
	score.setString("Blue   "+std::to_string(pointsLeftTeam_)+":"+std::to_string(pointsRightTeam_)+"   Red");
	score.setCharacterSize(80);
	score.move(685.f,1200.f);

	sf::CircleShape blueBox = sf::CircleShape(35);
	blueBox.setPosition(600,1250);
	blueBox.setFillColor(sf::Color(0,0,255));
	sf::CircleShape redBox = sf::CircleShape(35);
	redBox.setPosition(1300,1250);
	redBox.setFillColor(sf::Color(255,0,0));

	window->draw(scoreLine);
	window->draw(blueBox);
	window->draw(redBox);
	window->draw(score);

}

void Game::renderFpsDisplay(sf::RenderWindow* window, float value)
{
	sf::Text fpsString;
	sf::Font font;
	if (!font.loadFromFile("font.ttf")){
		std::cout<<"The sadness will last forever."<<std::endl;
	}
	fpsString.setFont(font);
	fpsString.setString(std::to_string(value) + " fps");
	fpsString.setCharacterSize(30);
	fpsString.move(1700.0f,0.0f);

	window->draw(fpsString);
}

void Game::updatePhysicalObjects()
{
	// update ball
	ball.frameUpdate();

	// update players
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->frameUpdate();	
	}
}

void Game::applyIntersectionPhysics()
{
	// for each player
	for (int i = 0; i < players.size(); ++i)
	{
		//check if shoot animation is needed
		if (players[i]->intersectsCircle(ball.getPosX(), ball.getPosY(), ball.getRadius(), true) && players[i]->inShootSequence())
		{
			applyShootingForce((*players[i]));
		}

		//check if player collides with ball
		if (players[i]->intersectsCircle(ball.getPosX(), ball.getPosY(), ball.getRadius(), false))
		{
			applyElasticImpact(ball, (*players[i]), 0.1 * players[i]->computeCurrentSpeed(), 1.0);
		}

		// check if player collides with other player
		for (int j = 0; j < i; ++j)
		{
			if (players[i]->intersectsCircle(players[j]->getPosX(), players[j]->getPosY(), players[j]->getRadius(), false))
			{
				applyElasticImpact( (*players[i]), (*players[j]), 1.0, 1.0);
			}
		}
	}
}

void Game::applyShootingForce(Player const& player)
{
	// apply shooting forces
	sf::Vector2f shootDir( ball.getPosX() - player.getPosX(), ball.getPosY() - player.getPosY() );
	float scaleFactor(0.2);
	ball.setVelocity(scaleFactor * shootDir.x, scaleFactor * shootDir.y);
}

void Game::applyElasticImpact(PhysicalObject& lhs, PhysicalObject& rhs, float lhsAbsorption, float rhsAbsorption)
{
	//elastic impact computation
	float lhsMass = lhs.getMass();
	float rhsMass = rhs.getMass();

	sf::Vector2f lhsVel(lhs.getVelX(), lhs.getVelY());
	sf::Vector2f rhsVel(rhs.getVelX(), rhs.getVelY());

	sf::Vector2f lhsVelAfterCollision( (lhsMass * lhsVel.x + rhsMass * (2 * rhsVel.x - lhsVel.x)) / (lhsMass + rhsMass),
										(lhsMass * lhsVel.y + rhsMass * (2 * rhsVel.y - lhsVel.y)) / (lhsMass + rhsMass));

	sf::Vector2f rhsVelAfterCollision( (rhsMass * rhsVel.x + lhsMass * (2 * lhsVel.x - rhsVel.x)) / (lhsMass + rhsMass),
	                                      (rhsMass * rhsVel.y + lhsMass * (2 * lhsVel.y - rhsVel.y)) / (lhsMass + rhsMass));
	

	//scale using absorption coefficients
	lhs.setVelocity(lhsAbsorption * lhsVelAfterCollision.x, lhsAbsorption * lhsVelAfterCollision.y);
	rhs.setVelocity(rhsAbsorption * rhsVelAfterCollision.x, rhsAbsorption * rhsVelAfterCollision.y);

	//pull rhs and lhs apart such that they can not overlap
	sf::Vector2f lhsCenter(lhs.getPosX(), lhs.getPosY());
	sf::Vector2f rhsCenter(rhs.getPosX(), rhs.getPosY());
	float lhsRadius(lhs.getRadius());
	float rhsRadius(rhs.getRadius());

	sf::Vector2f diffVec = rhsCenter - lhsCenter;
	float diffLength = std::sqrt(diffVec.x*diffVec.x + diffVec.y*diffVec.y);

	if (diffLength < lhsRadius + rhsRadius)
	{
		float incrementAtEachVectorEnd( (lhsRadius+rhsRadius-diffLength) / 2.0 );
		diffVec = diffVec / diffLength;
		diffVec = diffVec * (incrementAtEachVectorEnd);

		sf::Vector2f newBallPosition(lhsCenter - diffVec);
		sf::Vector2f newPlayerPosition(rhsCenter + diffVec);

		lhs.setPosition(newBallPosition.x, newBallPosition.y);
		rhs.setPosition(newPlayerPosition.x, newPlayerPosition.y);
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
				players[i]->resetToStart();
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
	players[playerID]->shoot();
}

void Game::movePlayer(int playerNumber, std::string direction)
{
	if (direction == "UP")
	{
		players[playerNumber]->moveUp();	
	}
	else if (direction == "DOWN")
	{
		players[playerNumber]->moveDown();	
	}
	else if (direction == "LEFT")
	{
		players[playerNumber]->moveLeft();	
	}
	else if (direction == "RIGHT")
	{
		players[playerNumber]->moveRight();	
	}
}