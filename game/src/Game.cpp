#include "Game.hpp"

Game::Game() : ballWasInLeftGoal_(false), ballWasInRightGoal_(false), 
			   framesToReset_(-1), pointsBlueTeam_(0), pointsRedTeam_(0),
			   numPlayersRed_(0), numPlayersBlue_(0)
{
	ball = new Ball();
}

Game::~Game()
{
	for (std::set<Player*>::iterator it = players.begin(); it != players.end(); ++it)
	{
		delete (*it);
	}

	delete ball;
}

Player* Game::addNewPlayer(std::string const& name, int number)
{
	//determine team color of new player
	//team with smaller number of players, if equal
	//team with smaller number of points, if equal
	//take blue team

	sf::Color teamColor;

	if (numPlayersBlue_ < numPlayersRed_)
	{
		teamColor = sf::Color(0, 0, 255);
	}
	else if (numPlayersBlue_ > numPlayersRed_)
	{
		teamColor = sf::Color(255, 0, 0);
	}
	else if (pointsBlueTeam_ < pointsRedTeam_)
	{
		teamColor = sf::Color(0, 0, 255);
	}
	else if (pointsBlueTeam_ > pointsRedTeam_)
	{
		teamColor = sf::Color(255, 0, 0);
	}
	else
	{
		teamColor = sf::Color(0, 0, 255);
	}

	//set spawn position dependent on team color
	sf::Vector2f spawnPosition(0,0);

	if (teamColor == sf::Color(0, 0, 255))
	{
		spawnPosition.x = 480;
		spawnPosition.y = 600;
		++numPlayersBlue_;
	}
	else if (teamColor == sf::Color(255, 0, 0))
	{
		spawnPosition.x = 1400;
		spawnPosition.y = 600;
		++numPlayersRed_;
	}

	Player* newPlayer = new Player(spawnPosition.x, spawnPosition.y, sf::Color(0,0,0), teamColor, name, number);
	players.insert(newPlayer);
	return newPlayer;
}

void Game::removePlayer(Player* playerToRemove)
{
	if (playerToRemove->getTeamColor() == sf::Color(255, 0, 0))
	{
		--numPlayersRed_;
	}
	else if (playerToRemove->getTeamColor() == sf::Color(0, 0, 255))
	{
		--numPlayersBlue_;
	}

	std::set<Player*>::iterator deleteIterator = players.find(playerToRemove);

	if (deleteIterator != players.end())
	{
		players.erase(deleteIterator);
		delete playerToRemove;
	}
}

void Game::renderBackground(sf::RenderWindow* window) 
{
	for (int i = 0; i < field_.size(); ++i)
	{
		window->draw(field_[i]);
	}
}

void Game::renderPlayers(sf::RenderWindow* window) 
{
	for (std::set<Player*>::iterator it = players.begin(); it != players.end(); ++it)
	{
		(*it)->render(window);
	}
}

void Game::renderBall(sf::RenderWindow* window)
{
	ball->render(window);
}

void Game::setScreenWidth(int in){
	screenWidth_ = in;
}

void Game::setScreenHeight(int in){
	screenHeight_ = in;
}


int Game::getScreenWidth(){
	return screenWidth_;
}

int Game::getScreenHeight(){
	return screenHeight_;
}

void Game::createField()
{
	createGreen();
	createFieldLines();
	createGoals();
	createScoreLine();
	createFpsDisplay();
}

void Game::calculateLinePoistions(){
	 leftLineAt = screenWidth_*0.0520833f;
	 topLineAt = screenHeight_*0.0192592f;
	 bottomLineAt = screenHeight_*0.86888f;
	 rightLineAt = screenWidth_*0.945312f;
	 centerLineAt = (screenWidth_*0.5)-3;


	 std::cout<<screenHeight_<<","<<screenWidth_;
	 std::cout<<"Left: "<<leftLineAt<<", Right: "<<rightLineAt<<", Bottom: "<<bottomLineAt<<", top: "<<topLineAt<<", center"<<centerLineAt<<std::endl;

}

void Game::createFieldLines(){
	calculateLinePoistions();
	sf::ConvexShape leftGoalLine;

	leftGoalLine.setPointCount(4);
	leftGoalLine.setPoint(0, sf::Vector2f(leftLineAt, topLineAt));
	leftGoalLine.setPoint(1, sf::Vector2f(leftLineAt, bottomLineAt));
	leftGoalLine.setPoint(2, sf::Vector2f(leftLineAt+5, bottomLineAt));
	leftGoalLine.setPoint(3, sf::Vector2f(leftLineAt+5, topLineAt));
	lines_.push_back(leftGoalLine);

	sf::ConvexShape rightGoalLine;
	rightGoalLine.setPointCount(4);
	rightGoalLine.setPoint(0, sf::Vector2f(rightLineAt, topLineAt));
	rightGoalLine.setPoint(1, sf::Vector2f(rightLineAt, bottomLineAt));
	rightGoalLine.setPoint(2, sf::Vector2f(rightLineAt+5, bottomLineAt));
	rightGoalLine.setPoint(3, sf::Vector2f(rightLineAt+5, topLineAt));
	lines_.push_back(rightGoalLine);


	sf::ConvexShape topSideLine;
	topSideLine.setPointCount(4);
	topSideLine.setPoint(0, sf::Vector2f(leftLineAt, topLineAt));
	topSideLine.setPoint(1, sf::Vector2f(rightLineAt, topLineAt));
	topSideLine.setPoint(2, sf::Vector2f(rightLineAt, topLineAt+5));
	topSideLine.setPoint(3, sf::Vector2f(leftLineAt, topLineAt+5));
	lines_.push_back(topSideLine);


	sf::ConvexShape bottomSideLine;
	bottomSideLine.setPointCount(4);
	bottomSideLine.setPoint(0, sf::Vector2f(rightLineAt, bottomLineAt+5));
	bottomSideLine.setPoint(1, sf::Vector2f(leftLineAt, bottomLineAt+5));
	bottomSideLine.setPoint(2, sf::Vector2f(leftLineAt, bottomLineAt));
	bottomSideLine.setPoint(3, sf::Vector2f(rightLineAt, bottomLineAt));
	lines_.push_back(bottomSideLine);


	sf::ConvexShape centerLine;
	centerLine.setPointCount(4);
	centerLine.setPoint(0, sf::Vector2f(centerLineAt, topLineAt));
	centerLine.setPoint(1, sf::Vector2f(centerLineAt, bottomLineAt));
	centerLine.setPoint(2, sf::Vector2f(centerLineAt+5, bottomLineAt));
	centerLine.setPoint(3, sf::Vector2f(centerLineAt+5, topLineAt));
	lines_.push_back(centerLine);

	int totalFieldHeight = bottomLineAt-topLineAt;
	double centerCirclePosY = totalFieldHeight/2;
	centerCirclePosY = centerCirclePosY+topLineAt;

	int totalFieldWidth = rightLineAt-leftLineAt;
	double centerCirclePosX = totalFieldWidth/2;
	centerCirclePosX = centerCirclePosX+leftLineAt;


	sf::CircleShape kickoffCircle = sf::CircleShape(185);
	kickoffCircle.setFillColor(sf::Color(255,255,255,0));
	kickoffCircle.setOutlineThickness(5);
	kickoffCircle.setOutlineColor(sf::Color(255,255,255));
	kickoffCircle.setOrigin(kickoffCircle.getRadius(), kickoffCircle.getRadius());
	kickoffCircle.setPosition(centerCirclePosX,centerCirclePosY);
	centerCircle_ = kickoffCircle;


	sf::CircleShape kickoffPoint = sf::CircleShape(10);
	kickoffPoint.setFillColor(sf::Color(255,255,255));
	kickoffPoint.setOutlineColor(sf::Color(255,255,255));
	kickoffPoint.setOrigin(kickoffPoint.getRadius(), kickoffPoint.getRadius());
	kickoffPoint.setPosition(centerCirclePosX,centerCirclePosY);
	centerPoint_ = kickoffPoint;

}

void Game::createGreen(){
	int pos = 0;
	int increment = screenWidth_/10;
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

		sf::RectangleShape rect(sf::Vector2f(pos+increment, screenHeight_));
		rect.setFillColor(currentGreen);
		rect.setPosition(pos, 0);
		field_.push_back(rect);
		pos+=increment;
	}
}

void Game::createGoals()
{
	goalLeft_ = sf::RectangleShape(sf::Vector2f(65,300));
	goalLeft_.setPosition(35,450);
	goalLeft_.setFillColor(sf::Color(0,0,0,0));
	goalLeft_.setOutlineThickness(5);
	goalLeft_.setOutlineColor(sf::Color(255,255,255));

	goalRight_ = sf::RectangleShape(sf::Vector2f(65,300));
	goalRight_.setPosition(1820,450);
	goalRight_.setFillColor(sf::Color(0,0,0,0));
	goalRight_.setOutlineThickness(5);
	goalRight_.setOutlineColor(sf::Color(255,255,255));
}

void Game::renderSidelines(sf::RenderWindow* window) 
{
	for(int i = 0; i < lines_.size(); ++i){
		window->draw(lines_[i]);
	}

	window->draw(centerCircle_);
	window->draw(centerPoint_);
}

void Game::renderGoals(sf::RenderWindow* window)
{
	window->draw(goalLeft_);
	window->draw(goalRight_);
}

void Game::renderScoreLine(sf::RenderWindow* window) 
{
	score_.setString("Blue   "+std::to_string(pointsBlueTeam_)+":"+std::to_string(pointsRedTeam_)+"   Red");
	window->draw(scoreLine_);
	window->draw(blueBox_);
	window->draw(redBox_);
	window->draw(score_);
}

void Game::renderFpsDisplay(sf::RenderWindow* window, float value)
{
	fpsString_.setString(std::to_string(value) + " fps");
	window->draw(fpsString_);
}

void Game::updatePhysicalObjects()
{
	// update ball
	ball->frameUpdate();

	// update players
	for (std::set<Player*>::iterator it = players.begin(); it != players.end(); ++it)
	{
		(*it)->frameUpdate();	
	}
}

void Game::applyIntersectionPhysics()
{
	// for each player
	for (std::set<Player*>::iterator it = players.begin(); it != players.end(); ++it)
	{
		Player* currentPlayer = *it;

		//check if shoot animation is needed
		if (currentPlayer->intersectsCircle(ball->getPosX(), ball->getPosY(), ball->getRadius(), true) && currentPlayer->inShootSequence())
		{
			applyShootingForce(currentPlayer);
		}

		//check if player collides with ball
		if (currentPlayer->intersectsCircle(ball->getPosX(), ball->getPosY(), ball->getRadius(), false))
		{
			applyElasticImpact(ball, currentPlayer, 0.1 * currentPlayer->computeCurrentSpeed(), 1.0);
		}

		// check if player collides with other player
		for (std::set<Player*>::iterator it2 = players.begin(); it2 != it; ++it2)
		{
			Player* otherPlayer = *it2;

			if (currentPlayer->intersectsCircle(otherPlayer->getPosX(), otherPlayer->getPosY(), otherPlayer->getRadius(), false))
			{
				applyElasticImpact( currentPlayer, otherPlayer, 1.0, 1.0);
			}
		}
	}
}

void Game::applyShootingForce(Player* player)
{
	// apply shooting forces
	sf::Vector2f shootDir( ball->getPosX() - player->getPosX(), ball->getPosY() - player->getPosY() );
	float scaleFactor(0.2);
	ball->setVelocity(scaleFactor * shootDir.x, scaleFactor * shootDir.y);
}

void Game::applyElasticImpact(PhysicalObject* lhs, PhysicalObject* rhs, float lhsAbsorption, float rhsAbsorption)
{
	//elastic impact computation
	float lhsMass = lhs->getMass();
	float rhsMass = rhs->getMass();

	sf::Vector2f lhsVel(lhs->getVelX(), lhs->getVelY());
	sf::Vector2f rhsVel(rhs->getVelX(), rhs->getVelY());

	sf::Vector2f lhsVelAfterCollision( (lhsMass * lhsVel.x + rhsMass * (2 * rhsVel.x - lhsVel.x)) / (lhsMass + rhsMass),
										(lhsMass * lhsVel.y + rhsMass * (2 * rhsVel.y - lhsVel.y)) / (lhsMass + rhsMass));

	sf::Vector2f rhsVelAfterCollision( (rhsMass * rhsVel.x + lhsMass * (2 * lhsVel.x - rhsVel.x)) / (lhsMass + rhsMass),
	                                      (rhsMass * rhsVel.y + lhsMass * (2 * lhsVel.y - rhsVel.y)) / (lhsMass + rhsMass));
	

	//scale using absorption coefficients
	lhs->setVelocity(lhsAbsorption * lhsVelAfterCollision.x, lhsAbsorption * lhsVelAfterCollision.y);
	rhs->setVelocity(rhsAbsorption * rhsVelAfterCollision.x, rhsAbsorption * rhsVelAfterCollision.y);

	//pull rhs and lhs apart such that they can not overlap
	sf::Vector2f lhsCenter(lhs->getPosX(), lhs->getPosY());
	sf::Vector2f rhsCenter(rhs->getPosX(), rhs->getPosY());
	float lhsRadius(lhs->getRadius());
	float rhsRadius(rhs->getRadius());

	sf::Vector2f diffVec = rhsCenter - lhsCenter;
	float diffLength = std::sqrt(diffVec.x*diffVec.x + diffVec.y*diffVec.y);

	if (diffLength < lhsRadius + rhsRadius)
	{
		float incrementAtEachVectorEnd( (lhsRadius+rhsRadius-diffLength) / 2.0 );
		diffVec = diffVec / diffLength;
		diffVec = diffVec * (incrementAtEachVectorEnd);

		sf::Vector2f newBallPosition(lhsCenter - diffVec);
		sf::Vector2f newPlayerPosition(rhsCenter + diffVec);

		lhs->setPosition(newBallPosition.x, newBallPosition.y);
		rhs->setPosition(newPlayerPosition.x, newPlayerPosition.y);
	}
}

void Game::checkForGoal()
{
	if (ball->isInLeftGoal() && ballWasInLeftGoal_ == false && framesToReset_ == -1)
	{
		++pointsRedTeam_;
		ballWasInLeftGoal_ = true;
		framesToReset_ = 100;
		ball->setColor(sf::Color(0,0,0));

		std::cout << std::endl;
		std::cout << "The left team scored a goal!" << std::endl;
		std::cout << "Current standings: Left " << pointsBlueTeam_ << " : " << pointsRedTeam_ << " Right" << std::endl;
		std::cout << std::endl;
	}
	else if (ball->isInRightGoal() && ballWasInRightGoal_ == false && framesToReset_ == -1)
	{
		++pointsBlueTeam_;
		ballWasInRightGoal_ = true;
		framesToReset_ = 100;
		ball->setColor(sf::Color(0,0,0));

		std::cout << std::endl;
		std::cout << "The left team scored a goal!" << std::endl;
		std::cout << "Current standings: Left " << pointsBlueTeam_ << " : " << pointsRedTeam_ << " Right" << std::endl;
		std::cout << std::endl;
	}
	else if (ball->isInLeftGoal() == false && ballWasInLeftGoal_ == true)
	{
		ballWasInLeftGoal_ = false;
	}
	else if (ball->isInRightGoal() == false && ballWasInRightGoal_ == true)
	{
		ballWasInRightGoal_ = false;
	}

	// count frames until ball is reset (after scoring animation)
	if (framesToReset_ > -1)
	{
		if (framesToReset_ == 0)
		{
			ball->resetToCenter();
			
			for (std::set<Player*>::iterator it = players.begin(); it != players.end(); ++it)
			{
				(*it)->resetToStart();
			}

			framesToReset_ = -1;
		}
		else
		{
			ball->setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
			--framesToReset_;
		}
	}
}





void Game::createScoreLine()
{
	//sf::RectangleShape scoreLine_ = sf::RectangleShape(0,1200,1920,1350,sf::Color(0,0,0));
	scoreLine_ = sf::RectangleShape(sf::Vector2f(1920,150));
	scoreLine_.setPosition(0,1200);
	scoreLine_.setFillColor(sf::Color(0,0,0));

	if (!font_.loadFromFile("font.ttf")){
		std::cout<<"The sadness will last forever."<<std::endl;
	}
	score_.setFont(font_);
	score_.setString("Blue   "+std::to_string(pointsBlueTeam_)+":"+std::to_string(pointsRedTeam_)+"   Red");
	score_.setCharacterSize(80);
	score_.move(685.f,1200.f);

	blueBox_ = sf::CircleShape(35);
	blueBox_.setPosition(600,1250);
	blueBox_.setFillColor(sf::Color(0,0,255));
	redBox_ = sf::CircleShape(35);
	redBox_.setPosition(1300,1250);
	redBox_.setFillColor(sf::Color(255,0,0));
}

void Game::createFpsDisplay()
{
	if (!font_.loadFromFile("font.ttf")){
		std::cout<<"The sadness will last forever."<<std::endl;
	}

	fpsString_.setFont(font_);
	fpsString_.setCharacterSize(30);
	fpsString_.move(100.0f,0.0f);
}