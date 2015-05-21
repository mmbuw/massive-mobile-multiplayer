#include "Game.hpp"

Game::Game(int screenWidth, int screenHeight) : ballWasInLeftGoal_(false),
               screenWidth_(screenWidth), screenHeight_(screenHeight), ballWasInRightGoal_(false), 
			   goalAnimationDurationSec_(4), pointsBlueTeam_(0), pointsRedTeam_(0),
			   numPlayersRed_(0), numPlayersBlue_(0), inGoalAnimation_(false)
{}

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
		spawnPosition.x = 0.06 * screenWidth_;
		spawnPosition.y = 100 + (rand() % (screenHeight_-200));
		++numPlayersBlue_;
	}
	else if (teamColor == sf::Color(255, 0, 0))
	{
		spawnPosition.x = 0.94 * screenWidth_;
		spawnPosition.y = 100 + (rand() % (screenHeight_-200));
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


int Game::getScreenWidth(){
	return screenWidth_;
}

int Game::getScreenHeight(){
	return screenHeight_;
}

void Game::createField()
{
	
	createFieldLines();
	createGreen();
	sf::Vector2f fieldCenter = centerCircle_.getPosition();
	createBall(fieldCenter.x, fieldCenter.y);
		 ball->setAllLines(leftLineAt,rightLineAt,topLineAt,bottomLineAt);
	createGoals();
	createScoreLine();
	createFpsDisplay();
}

void Game::calculateLinePoistions(){
	 leftLineAt = screenWidth_*0.0520833f;
	 topLineAt = screenHeight_*0.0192592f;
	 bottomLineAt = screenHeight_*0.86888f;
	 rightLineAt = screenWidth_*0.925312f;
	 centerLineAt = ((rightLineAt-leftLineAt)/2)+leftLineAt;


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
	bottomSideLine.setPoint(0, sf::Vector2f(rightLineAt+5, bottomLineAt+5));
	bottomSideLine.setPoint(1, sf::Vector2f(leftLineAt, bottomLineAt+5));
	bottomSideLine.setPoint(2, sf::Vector2f(leftLineAt, bottomLineAt));
	bottomSideLine.setPoint(3, sf::Vector2f(rightLineAt+5, bottomLineAt));
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

void Game::createBall(int startX, int startY)
{
	ball = new Ball(startX, startY);
}

void Game::createGreen(){
	int pos = centerLineAt;
	int increment = screenWidth_/10;
	sf::Color lightGreen = sf::Color(50,200,50);
	sf::Color darkGreen = sf::Color(75,220,75);



	sf::RectangleShape rect(sf::Vector2f(increment, screenHeight_));

	for (int i = 0; i < 7; ++i){
		rect.setPosition(pos+increment*i,0);
		if (i%2 == 0){
			rect.setFillColor(darkGreen);
		}else {
			rect.setFillColor(lightGreen);
		}

		field_.push_back(rect);
	}

	for (int i = 1; i < 6; ++i){
		rect.setPosition(pos-increment*i,0);
		if (i%2 == 0){
			rect.setFillColor(darkGreen);
		}else {
			rect.setFillColor(lightGreen);
		}

		field_.push_back(rect);
	}
}

void Game::createGoals()
{
	double middleoflines = ((bottomLineAt-topLineAt)/2)+topLineAt;
	double goalsStartAt = middleoflines - (0.2*(middleoflines));

	goalLeft_ = sf::RectangleShape(sf::Vector2f(0.03385416*screenWidth_,0.3*screenHeight_));
	goalLeft_.setPosition(0.01822916*screenWidth_,middleoflines-(0.15*screenHeight_));
	goalLeft_.setFillColor(sf::Color(0,0,0,0));
	goalLeft_.setOutlineThickness(5);
	goalLeft_.setOutlineColor(sf::Color(255,255,255));

	goalRight_ = sf::RectangleShape(sf::Vector2f(0.03385416*screenWidth_,0.3*screenHeight_));
	goalRight_.setPosition(0.92791616666*screenWidth_,middleoflines-(0.15*screenHeight_));
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
	score_.setString(std::to_string(pointsBlueTeam_)+":"+std::to_string(pointsRedTeam_));
	blue_.setString("Blue");
	red_.setString("Red");

	window->draw(scoreLine_);
	window->draw(blueBox_);
	window->draw(redBox_);
	window->draw(score_);
	window->draw(blue_);
	window->draw(red_);

	if (inGoalAnimation_)
	{
		window->draw(goalTextOne_);
		window->draw(goalTextTwo_);
	}
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
			applyElasticImpact(ball, currentPlayer, 0.1 * currentPlayer->computeCurrentSpeed(), 1.0, false);
		}

		// check if player collides with other player
		for (std::set<Player*>::iterator it2 = players.begin(); it2 != it; ++it2)
		{
			Player* otherPlayer = *it2;

			if (currentPlayer->intersectsCircle(otherPlayer->getPosX(), otherPlayer->getPosY(), otherPlayer->getRadius(), false))
			{
				applyElasticImpact( currentPlayer, otherPlayer, 1.0, 1.0, true);
			}
		}
	}
}

void Game::applyShootingForce(Player* player)
{
	// apply shooting forces
	sf::Vector2f currentBallPos = sf::Vector2f(ball->getPosX(), ball->getPosY());
	sf::Vector2f shootDir( currentBallPos.x - player->getPosX(), currentBallPos.y - player->getPosY() );
	float scaleFactor(std::max(ball->computeCurrentSpeed() * 0.001 + 0.2, 0.2));

	// tear ball away from collision
	ball->setPosition(currentBallPos.x + 0.1 * shootDir.x, currentBallPos.y + 0.1 * shootDir.y);
	ball->setVelocity(scaleFactor * shootDir.x, scaleFactor * shootDir.y);
}

//If equal is false: when checking for collision with ball, lhs must be the ball as it is controlled by the player (rhs)
void Game::applyElasticImpact(PhysicalObject* lhs, PhysicalObject* rhs, float lhsAbsorption, float rhsAbsorption, bool equal)
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

	//if players are exactly at the same positon (e.g. during spawn), move them apart
	sf::Vector2f diffVec;
	if (lhsCenter == rhsCenter)
	{
		diffVec = sf::Vector2f(1.0, 0.0);
	}
	else
	{
		diffVec = rhsCenter - lhsCenter;
	}

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

		if ( !equal && rhs->computeCurrentSpeed() > lhs->computeCurrentSpeed())
		{
			sf::Vector2f newDirection(rhs->getVelX() - diffVec.x, rhs->getVelY() - diffVec.y);
			float magnitude = std::sqrt(newDirection.x*newDirection.x + newDirection.y*newDirection.y);
			newDirection = newDirection / magnitude;
			newDirection = newDirection * rhs->computeCurrentSpeed();
			lhs->setVelocity(newDirection.x, newDirection.y);
		}
	}
}

void Game::checkForGoal()
{
	if (ball->isInLeftGoal() && ballWasInLeftGoal_ == false && inGoalAnimation_ == false)
	{
		++pointsRedTeam_;
		ballWasInLeftGoal_ = true;
		goalAnimationStartTime_ = Clock::now();
		inGoalAnimation_ = true;
		ball->setColor(sf::Color(0,0,0));

		//std::cout << std::endl;
		//std::cout << "The red team scored a goal!" << std::endl;
		//std::cout << "Current standings: Left " << pointsBlueTeam_ << " : " << pointsRedTeam_ << " Right" << std::endl;
		//std::cout << std::endl;

		goalTextOne_.setString("Red scores");
		goalTextOne_.setColor(sf::Color(255, 0, 0));
		goalTextTwo_.setColor(sf::Color(255, 0, 0));
		celebratingTeam_ = sf::Color(255,0,0);
	}
	else if (ball->isInRightGoal() && ballWasInRightGoal_ == false && inGoalAnimation_ == false)
	{
		++pointsBlueTeam_;
		ballWasInRightGoal_ = true;
		goalAnimationStartTime_ = Clock::now();
		inGoalAnimation_ = true;
		ball->setColor(sf::Color(0,0,0));

		//std::cout << std::endl;
		//std::cout << "The blue team scored a goal!" << std::endl;
		//std::cout << "Current standings: Left " << pointsBlueTeam_ << " : " << pointsRedTeam_ << " Right" << std::endl;
		//std::cout << std::endl;

		goalTextOne_.setString("Blue scores");
		goalTextOne_.setColor(sf::Color(0, 0, 255));
		goalTextTwo_.setColor(sf::Color(0, 0, 255));
		celebratingTeam_ = sf::Color(0,0,255);
	}
	else if (ball->isInLeftGoal() == false && ballWasInLeftGoal_ == true)
	{
		ballWasInLeftGoal_ = false;
	}
	else if (ball->isInRightGoal() == false && ballWasInRightGoal_ == true)
	{
		ballWasInRightGoal_ = false;
	}

	//perform goal animation
	if (inGoalAnimation_)
	{
		Clock::time_point nowTime(Clock::now());
		int elapsedMilliseconds = (std::chrono::duration_cast<milliseconds>(nowTime - goalAnimationStartTime_)).count();

		if (elapsedMilliseconds > goalAnimationDurationSec_ * 1000)
		{
			ball->resetToStart();
			resetPlayers();
			inGoalAnimation_ = false;
			goalTextOne_.setString("");
			goalTextTwo_.setString("");
		}
		else
		{
			//perform text animation
			float timeStep = elapsedMilliseconds / ((goalAnimationDurationSec_-1)*1000.0);
			timeStep = mapTimeStep(timeStep);

			sf::FloatRect textRect = goalTextOne_.getGlobalBounds();
			float textWidth = textRect.width;
			float textHeight = textRect.height;

			sf::Vector2f lerpStart(sf::Vector2f(-textWidth, screenHeight_*0.3 - textHeight/2.0));
			sf::Vector2f lerpEnd(sf::Vector2f(screenWidth_, screenHeight_*0.3 - textHeight/2.0));
			sf::Vector2f currentTextPos = lerp(lerpStart, lerpEnd, timeStep);
			goalTextOne_.setPosition(currentTextPos);

			if (celebratingTeam_ == sf::Color(255,0,0))
			{
				if (timeStep < 0.5)
					goalTextTwo_.setString(std::to_string(pointsRedTeam_-1));
				else
					goalTextTwo_.setString(std::to_string(pointsRedTeam_));
			}
			else
			{
				if (timeStep < 0.5)
					goalTextTwo_.setString(std::to_string(pointsBlueTeam_-1));
				else
					goalTextTwo_.setString(std::to_string(pointsBlueTeam_));
			}

			textRect = goalTextTwo_.getGlobalBounds();
			textWidth = textRect.width;
			textHeight = textRect.height;

			lerpStart = sf::Vector2f(screenWidth_, screenHeight_*0.5 - textHeight/2.0);
			lerpEnd = sf::Vector2f(-textWidth, screenHeight_*0.5 - textHeight/2.0);
			currentTextPos = lerp(lerpStart, lerpEnd, timeStep);
			goalTextTwo_.setPosition(currentTextPos);

			//perform blinking ball animation
			ball->setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
		}
	}

}

void Game::resetPlayers()
{
	int currentBluePlayer(0);
	int currentRedPlayer(0);

	float redSectorSize(180.0/numPlayersRed_);
	float blueSectorSize(180.0/numPlayersBlue_);

	float radiusFromCenter(screenWidth_/5.0);

	for (std::set<Player*>::iterator it = players.begin(); it != players.end(); ++it)
	{
		sf::Color teamColor = (*it)->getTeamColor();
		float angleDegrees;
		int xDirectionFactor;

		if (teamColor == sf::Color(255, 0, 0))
		{
			angleDegrees = 90.0 - redSectorSize/2.0 - currentRedPlayer * redSectorSize;
			xDirectionFactor = 1;
			++currentRedPlayer;
		}
		else if (teamColor == sf::Color(0, 0, 255))
		{
			angleDegrees = 90.0 - blueSectorSize/2.0 - currentBluePlayer * blueSectorSize;
			xDirectionFactor = -1;
			++currentBluePlayer;
		}

		float vecFromCenterX = xDirectionFactor * std::cos(angleDegrees * M_PI / 180.0);
		float vecFromCenterY = std::sqrt(1 - vecFromCenterX*vecFromCenterX);

		if (angleDegrees < 0)
			vecFromCenterY = -vecFromCenterY;

		sf::Vector2f center = centerPoint_.getPosition();

		(*it)->setPosition(center.x + radiusFromCenter * vecFromCenterX,
			               center.y + radiusFromCenter * vecFromCenterY);
		(*it)->setVelocity(0.0, 0.0);

	}

}


void Game::createScoreLine()
{
	//sf::RectangleShape scoreLine_ = sf::RectangleShape(0,1200,1920,1350,sf::Color(0,0,0));
	scoreLine_ = sf::RectangleShape(sf::Vector2f(screenWidth_,0.1153846*screenHeight_));
	scoreLine_.setPosition(0,0.92307692307*screenHeight_);
	scoreLine_.setFillColor(sf::Color(0,0,0));

	//create text
	if (!font_.loadFromFile("resources/blox.ttf"))
	{
		std::cout << "[Game.cpp] Error loading font." << std::endl;
	}

	if (!scoreFont_.loadFromFile("resources/blox.ttf"))
	{
		std::cout << "[Game.cpp] Error loading score font." << std::endl;
	}

	score_.setFont(font_);
	blue_.setFont(font_);
	red_.setFont(font_);
	goalTextOne_.setFont(scoreFont_);
	goalTextTwo_.setFont(scoreFont_);

	score_.setString(std::to_string(pointsBlueTeam_)+":"+std::to_string(pointsRedTeam_));
	blue_.setString("Blue");
	red_.setString("Red");


	score_.setCharacterSize(screenHeight_*0.06153846153);
	blue_.setCharacterSize(screenHeight_*0.06153846153);
	red_.setCharacterSize(screenHeight_*0.06153846153);
	goalTextOne_.setCharacterSize(screenHeight_*0.15);
	goalTextTwo_.setCharacterSize(screenHeight_*0.15);


	blue_.move(0.3*screenWidth_,0.92307692307*screenHeight_);
	red_.move(0.6*screenWidth_,0.92307692307*screenHeight_);
	score_.move(0.45*screenWidth_,0.92307692307*screenHeight_);


	blueBox_ = sf::CircleShape(0.02692307692*screenHeight_);
	blueBox_.setPosition(0.20*screenWidth_,0.93*screenHeight_);
	blueBox_.setFillColor(sf::Color(0,0,255));
	redBox_ = sf::CircleShape(0.02692307692*screenHeight_);
	redBox_.setPosition(0.7*screenWidth_,0.93*screenHeight_);
	redBox_.setFillColor(sf::Color(255,0,0));
}

void Game::createFpsDisplay()
{
	if (!font_.loadFromFile("resources/font.ttf"))
	{
		std::cout << "[Game.cpp] Error loading font." << std::endl;
	}

	fpsString_.setFont(font_);
	fpsString_.setCharacterSize(30);
	fpsString_.move(100.0f,0.0f);
}

sf::Vector2f const lerp(sf::Vector2f const& start, sf::Vector2f const& end, float timeStep)
{
	float factor;

	if (timeStep < 0.0)
		factor = 0.0;
	if (timeStep > 1.0)
		factor = 1.0;
	else
		factor = timeStep;

	sf::Vector2f diffVec = end - start;
	return sf::Vector2f(start.x + factor * diffVec.x, start.y + factor * diffVec.y);
}

float mapTimeStep(float inputTimeStep)
{
	return 0.5 * (std::pow(2*inputTimeStep-1, 3) + 1);
}