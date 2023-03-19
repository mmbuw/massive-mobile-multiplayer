#include "Game.hpp"

Game::Game(int screenWidth, int screenHeight, std::string resourcesPath) : ballWasInLeftGoal_(false),
               screenWidth_(screenWidth), screenHeight_(screenHeight), ballWasInRightGoal_(false), 
			   pointsBlueTeam_(0), pointsRedTeam_(0), goalAnimationScoreUpdateDone_(false),
			   numPlayersRed_(0), numPlayersBlue_(0), inGoalAnimation_(false), inEndAnimation_(false),
			   resourcesPath_(resourcesPath)
{}


Game::~Game()
{
	for (std::set<Player*>::iterator it = players_.begin(); it != players_.end(); ++it)
	{
		delete (*it);
	}

	delete ball_;
}


Player* Game::addNewPlayer(std::string const& name)
{
	//determine team color of new player
	//team with smaller number of players_, if equal
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

	//set spawn position dependend on team color
	//get a player number
	sf::Vector2f spawnPosition(0,0);
	int number;

	if (teamColor == sf::Color(0, 0, 255))
	{
		spawnPosition.x = 0.06 * screenWidth_;
		spawnPosition.y = ((int) screenHeight_*0.1) + (rand() % (int) (screenHeight_*0.7));
		++numPlayersBlue_;

		do
		{
			number = (rand()%99) + 1;
		} 
		while (blueNumbersInUse_[number] == true);

		blueNumbersInUse_[number] = true;
	}
	else if (teamColor == sf::Color(255, 0, 0))
	{
		spawnPosition.x = 0.94 * screenWidth_;
		spawnPosition.y = ((int) screenHeight_*0.1) + (rand() % (int) (screenHeight_*0.7));
		++numPlayersRed_;

		do
		{
			number = (rand()%99) + 1;
		} 
		while (redNumbersInUse_[number] == true);

		redNumbersInUse_[number] = true;
	}

	Player* newPlayer = new Player(spawnPosition.x, spawnPosition.y, sf::Color(0,0,0), teamColor, name, number);
	
	//adjust player radius to other players
	float newRadius;

	if (players_.size() > 0)
	{
		Player* firstPlayer = *(players_.begin());
		newRadius = firstPlayer->getRadius();
	}
	else
	{
		newRadius = getPlayerRadius();
	}
			
    newPlayer->setRadius(newRadius);
	players_.insert(newPlayer);
	return newPlayer;
}


void Game::removePlayer(Player* playerToRemove)
{
	if (playerToRemove->getTeamColor() == sf::Color(255, 0, 0))
	{
		--numPlayersRed_;
		redNumbersInUse_[playerToRemove->getShirtNumber()] = false;
	}
	else if (playerToRemove->getTeamColor() == sf::Color(0, 0, 255))
	{
		--numPlayersBlue_;
		blueNumbersInUse_[playerToRemove->getShirtNumber()] = false;
	}

	std::set<Player*>::iterator deleteIterator = players_.find(playerToRemove);

	if (deleteIterator != players_.end())
	{
		players_.erase(deleteIterator);
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
	for (std::set<Player*>::iterator it = players_.begin(); it != players_.end(); ++it)
	{
		(*it)->render(window);
	}
}


void Game::renderBall(sf::RenderWindow* window)
{
	ball_->render(window);
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

	if (inGoalAnimation_ || inEndAnimation_)
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

void Game::renderTime(sf::RenderWindow* window) 
{
	sf::Time elapsed = clock_.getElapsedTime();
	int inSeconds = elapsed.asSeconds();
	int remainingSeconds = (GAME_DURATION_MINUTES * 60) - inSeconds;

	if (remainingSeconds <= 0 && inEndAnimation_ == false && inGoalAnimation_ == false)
	{
		//prepare game end animation
		inEndAnimation_ = true;
		endAnimationStartTime_ = Clock::now();

		if (pointsRedTeam_ > pointsBlueTeam_)
		{
			goalTextOne_.setFillColor(sf::Color(255,0,0));
			goalTextOne_.setString("red wins");
			goalTextTwo_.setFillColor(sf::Color(255,0,0));

		}
		else if (pointsBlueTeam_ > pointsRedTeam_)
		{
			goalTextOne_.setFillColor(sf::Color(0,0,255));
			goalTextOne_.setString("blue wins");
			goalTextTwo_.setFillColor(sf::Color(0,0,255));
		}
		else
		{
			goalTextOne_.setFillColor(sf::Color(0,0,0));
			goalTextOne_.setString("draw");
			goalTextTwo_.setFillColor(sf::Color(0,0,0));
		}

		goalTextTwo_.setString(std::to_string(pointsBlueTeam_) + " to " + std::to_string(pointsRedTeam_));
	}

	if (remainingSeconds < 0)
		remainingSeconds = 0;

	int seconds = remainingSeconds%60;
	int minutes = (remainingSeconds-seconds)/60;

	std::string minuteString = std::to_string(minutes);
	std::string secondString = std::to_string(seconds);

	if (minutes < 10){
		minuteString = std::to_string(0)+minuteString;
	}
	if (seconds < 10){
		secondString = std::to_string(0)+secondString;
	}

	time_.setString(minuteString+":"+secondString);

	if (remainingSeconds <= 10)
	{
		time_.setFillColor(sf::Color(255,255,0));
	}
	else
	{
		time_.setFillColor(sf::Color(255,255,255));
	}

	if (inEndAnimation_)
		performEndOfGameAnimation();

	window->draw(time_);
}

float Game::getPlayerRadius() const
{
	//new size when the field gets full
	if (numPlayersRed_ + numPlayersBlue_ < 5)
		return 35.0;
	else if (numPlayersRed_ + numPlayersBlue_ < 9)
		return 30.0;
	else
		return 25.0;
}

void Game::updatePhysicalCircles()
{
	// update ball_
	ball_->frameUpdate();

	// update players_
	for (std::set<Player*>::iterator it = players_.begin(); it != players_.end(); ++it)
	{
		(*it)->frameUpdate();	
	}
}

void Game::applyIntersectionPhysics()
{
	// for each player
	for (std::set<Player*>::iterator it = players_.begin(); it != players_.end(); ++it)
	{
		Player* currentPlayer = *it;

		//check if shoot animation is needed
		if (currentPlayer->intersectsCircle(ball_->getPosX(), ball_->getPosY(), ball_->getRadius(), true) && currentPlayer->inShootSequence())
		{
			applyShootingForce(currentPlayer);
			ball_->setLastPlayerTouch(currentPlayer);
		}

		//check if player collides with ball_
		if (currentPlayer->intersectsCircle(ball_->getPosX(), ball_->getPosY(), ball_->getRadius(), false))
		{
			applyElasticImpact(ball_, currentPlayer, 0.1 * currentPlayer->computeCurrentSpeed(), 1.0, false);
			ball_->setLastPlayerTouch(currentPlayer);
		}

		// check if player collides with other player
		for (std::set<Player*>::iterator it2 = players_.begin(); it2 != it; ++it2)
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
	sf::Vector2f currentBallPos = sf::Vector2f(ball_->getPosX(), ball_->getPosY());
	sf::Vector2f shootDir( currentBallPos.x - player->getPosX(), currentBallPos.y - player->getPosY() );
	float scaleFactor(std::max(ball_->computeCurrentSpeed() * 0.001 + 0.2, 0.2));

	// tear ball_ away from collision
	ball_->setPosition(currentBallPos.x + 0.1 * shootDir.x, currentBallPos.y + 0.1 * shootDir.y);
	ball_->setVelocity(scaleFactor * shootDir.x, scaleFactor * shootDir.y);
}

//If equal is false: when checking for collision with ball_, lhs must be the ball_ as it is controlled by the player (rhs)
void Game::applyElasticImpact(PhysicalCircle* lhs, PhysicalCircle* rhs, float lhsAbsorption, float rhsAbsorption, bool equal)
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

	//if players_ are exactly at the same positon (e.g. during spawn), move them apart
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
		//used for player-player collision
		if (equal)
		{
			float incrementAtEachVectorEnd( (lhsRadius+rhsRadius-diffLength) / 2.0 );
			diffVec = diffVec / diffLength;
			diffVec = diffVec * (incrementAtEachVectorEnd);
			sf::Vector2f newLhsPosition(lhsCenter - diffVec);
			sf::Vector2f newRhsPosition(rhsCenter + diffVec);
			lhs->setPosition(newLhsPosition.x, newLhsPosition.y);
			rhs->setPosition(newRhsPosition.x, newRhsPosition.y);
		}
		//used for ball_-player collision
		else
		{
			diffVec = diffVec / diffLength;
			diffVec = diffVec * (lhsRadius+rhsRadius);
			sf::Vector2f newLhsPosition(rhsCenter - diffVec);
			lhs->setPosition(newLhsPosition.x, newLhsPosition.y);

			sf::Vector2f newDirection = -diffVec;
			float magnitude = lhs->computeCurrentSpeed();
			
			if (magnitude != 0.0)
			{
				//factor how strong ball_s bounce off the player when she just pushes the ball_
				float pushBoostFactor(0.03);
				newDirection = newDirection * rhs->computeCurrentSpeed();
				lhs->setVelocity(newDirection.x * pushBoostFactor, newDirection.y * pushBoostFactor);
			}
		}
	}
}

void Game::performEndOfGameAnimation()
{
	Clock::time_point nowTime(Clock::now());
	int elapsedMilliseconds = (std::chrono::duration_cast<milliseconds>(nowTime - endAnimationStartTime_)).count();
	
	if (elapsedMilliseconds > END_ANIMATION_DURATION_SEC * 1000)
	{
		ball_->resetToStart();
		resetPlayers();
		resetScore();
		clock_.restart();
		inEndAnimation_ = false;
	}
	else
	{
		float timeStep = elapsedMilliseconds / ((END_ANIMATION_DURATION_SEC)*1000.0);
		timeStep = mapTimeStep(timeStep);

		sf::FloatRect textRect = goalTextOne_.getGlobalBounds();
		float textWidth = textRect.width;
		float textHeight = textRect.height;

		sf::Vector2f lerpStart(sf::Vector2f(-textWidth, screenHeight_*0.3 - textHeight/2.0));
		sf::Vector2f lerpEnd(sf::Vector2f(screenWidth_, screenHeight_*0.3 - textHeight/2.0));
		sf::Vector2f currentTextPos = lerp(lerpStart, lerpEnd, timeStep);
		goalTextOne_.setPosition(currentTextPos);

		textRect = goalTextTwo_.getGlobalBounds();
		textWidth = textRect.width;
		textHeight = textRect.height;

		lerpStart = sf::Vector2f(screenWidth_, screenHeight_*0.5 - textHeight/2.0);
		lerpEnd = sf::Vector2f(-textWidth, screenHeight_*0.5 - textHeight/2.0);
		currentTextPos = lerp(lerpStart, lerpEnd, timeStep);
		goalTextTwo_.setPosition(currentTextPos);
	}
}


void Game::resetScore()
{
	pointsBlueTeam_ = 0;
	pointsRedTeam_ = 0;
}


void Game::checkForGoal()
{
	if (ball_->isInLeftGoal() && ballWasInLeftGoal_ == false && inGoalAnimation_ == false && inEndAnimation_ == false)
	{
		++pointsRedTeam_;
		ballWasInLeftGoal_ = true;
		goalAnimationStartTime_ = Clock::now();
		inGoalAnimation_ = true;
		ball_->setColor(sf::Color(0,0,0));

		goalTextOne_.setString(ball_->getLastPlayerTouchName() + " scores");
		goalTextOne_.setFillColor(sf::Color(255, 0, 0));
		goalTextTwo_.setFillColor(sf::Color(255, 0, 0));
		celebratingTeam_ = sf::Color(255,0,0);

		goalTextTwo_.setString(std::to_string(pointsRedTeam_-1));
		goalAnimationScoreUpdateDone_ = false;
	}
	else if (ball_->isInRightGoal() && ballWasInRightGoal_ == false && inGoalAnimation_ == false && inEndAnimation_ == false)
	{
		++pointsBlueTeam_;
		ballWasInRightGoal_ = true;
		goalAnimationStartTime_ = Clock::now();
		inGoalAnimation_ = true;
		ball_->setColor(sf::Color(0,0,0));

		goalTextOne_.setString(ball_->getLastPlayerTouchName() + " scores");
		goalTextOne_.setFillColor(sf::Color(0, 0, 255));
		goalTextTwo_.setFillColor(sf::Color(0, 0, 255));
		celebratingTeam_ = sf::Color(0,0,255);

		goalTextTwo_.setString(std::to_string(pointsBlueTeam_-1));
		goalAnimationScoreUpdateDone_ = false;
	}
	else if (ball_->isInLeftGoal() == false && ballWasInLeftGoal_ == true)
	{
		ballWasInLeftGoal_ = false;
	}
	else if (ball_->isInRightGoal() == false && ballWasInRightGoal_ == true)
	{
		ballWasInRightGoal_ = false;
	}

	//perform goal animation
	if (inGoalAnimation_)
	{
		Clock::time_point nowTime(Clock::now());
		int elapsedMilliseconds = (std::chrono::duration_cast<milliseconds>(nowTime - goalAnimationStartTime_)).count();

		if (elapsedMilliseconds > GOAL_ANIMATION_DURATION_SEC * 1000)
		{
			ball_->resetToStart();
			resetPlayers();
			inGoalAnimation_ = false;
			goalTextOne_.setString("");
			goalTextTwo_.setString("");
		}
		else
		{
			//perform text animation
			float timeStep = elapsedMilliseconds / ((GOAL_ANIMATION_DURATION_SEC-1)*1000.0);
			timeStep = mapTimeStep(timeStep);

			sf::FloatRect textRect = goalTextOne_.getGlobalBounds();
			float textWidth = textRect.width;
			float textHeight = textRect.height;

			sf::Vector2f lerpStart(sf::Vector2f(-textWidth, screenHeight_*0.3 - textHeight/2.0));
			sf::Vector2f lerpEnd(sf::Vector2f(screenWidth_, screenHeight_*0.3 - textHeight/2.0));
			sf::Vector2f currentTextPos = lerp(lerpStart, lerpEnd, timeStep);
			goalTextOne_.setPosition(currentTextPos);

			if (timeStep > 0.5 && goalAnimationScoreUpdateDone_ == false)
			{
				goalAnimationScoreUpdateDone_ = true;

				if (celebratingTeam_ == sf::Color(255,0,0))
					goalTextTwo_.setString(std::to_string(pointsRedTeam_));
				else if (celebratingTeam_ == sf::Color(0,0,255))
					goalTextTwo_.setString(std::to_string(pointsBlueTeam_));

			}

			textRect = goalTextTwo_.getGlobalBounds();
			textWidth = textRect.width;
			textHeight = textRect.height;

			lerpStart = sf::Vector2f(screenWidth_, screenHeight_*0.5 - textHeight/2.0);
			lerpEnd = sf::Vector2f(-textWidth, screenHeight_*0.5 - textHeight/2.0);
			currentTextPos = lerp(lerpStart, lerpEnd, timeStep);
			goalTextTwo_.setPosition(currentTextPos);

			//perform blinking ball_ animation
			ball_->setColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
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

	float newPlayerRadius = getPlayerRadius();

	for (std::set<Player*>::iterator it = players_.begin(); it != players_.end(); ++it)
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
		(*it)->setRadius(newPlayerRadius);
	}
}


void Game::createField()
{
	initTime();
	createFieldLines();
	createGreen();
	sf::Vector2f fieldCenter = centerCircle_.getPosition();
	createBall(fieldCenter.x, fieldCenter.y);
	ball_->setAllLines(screenWidth_, screenHeight_, leftLineAt_,rightLineAt_,topLineAt_,bottomLineAt_);
	createGoals();
	createScoreLine();
	createFpsDisplay();
}


int Game::getScreenWidth()
{
	return screenWidth_;
}

int Game::getScreenHeight()
{
	return screenHeight_;
}


void Game::createFieldLines()
{
	calculateLinePoistions();
	sf::ConvexShape leftGoalLine;

	leftGoalLine.setPointCount(4);
	leftGoalLine.setPoint(0, sf::Vector2f(leftLineAt_, topLineAt_));
	leftGoalLine.setPoint(1, sf::Vector2f(leftLineAt_, bottomLineAt_));
	leftGoalLine.setPoint(2, sf::Vector2f(leftLineAt_+5, bottomLineAt_));
	leftGoalLine.setPoint(3, sf::Vector2f(leftLineAt_+5, topLineAt_));
	lines_.push_back(leftGoalLine);

	sf::ConvexShape rightGoalLine;
	rightGoalLine.setPointCount(4);
	rightGoalLine.setPoint(0, sf::Vector2f(rightLineAt_, topLineAt_));
	rightGoalLine.setPoint(1, sf::Vector2f(rightLineAt_, bottomLineAt_));
	rightGoalLine.setPoint(2, sf::Vector2f(rightLineAt_+5, bottomLineAt_));
	rightGoalLine.setPoint(3, sf::Vector2f(rightLineAt_+5, topLineAt_));
	lines_.push_back(rightGoalLine);


	sf::ConvexShape topSideLine;
	topSideLine.setPointCount(4);
	topSideLine.setPoint(0, sf::Vector2f(leftLineAt_, topLineAt_));
	topSideLine.setPoint(1, sf::Vector2f(rightLineAt_, topLineAt_));
	topSideLine.setPoint(2, sf::Vector2f(rightLineAt_, topLineAt_+5));
	topSideLine.setPoint(3, sf::Vector2f(leftLineAt_, topLineAt_+5));
	lines_.push_back(topSideLine);


	sf::ConvexShape bottomSideLine;
	bottomSideLine.setPointCount(4);
	bottomSideLine.setPoint(0, sf::Vector2f(rightLineAt_+5, bottomLineAt_+5));
	bottomSideLine.setPoint(1, sf::Vector2f(leftLineAt_, bottomLineAt_+5));
	bottomSideLine.setPoint(2, sf::Vector2f(leftLineAt_, bottomLineAt_));
	bottomSideLine.setPoint(3, sf::Vector2f(rightLineAt_+5, bottomLineAt_));
	lines_.push_back(bottomSideLine);


	sf::ConvexShape centerLine;
	centerLine.setPointCount(4);
	centerLine.setPoint(0, sf::Vector2f(centerLineAt_, topLineAt_));
	centerLine.setPoint(1, sf::Vector2f(centerLineAt_, bottomLineAt_));
	centerLine.setPoint(2, sf::Vector2f(centerLineAt_+5, bottomLineAt_));
	centerLine.setPoint(3, sf::Vector2f(centerLineAt_+5, topLineAt_));
	lines_.push_back(centerLine);

	int totalFieldHeight = bottomLineAt_-topLineAt_;
	double centerCirclePosY = totalFieldHeight/2;
	centerCirclePosY = centerCirclePosY+topLineAt_;

	int totalFieldWidth = rightLineAt_-leftLineAt_;
	double centerCirclePosX = totalFieldWidth/2;
	centerCirclePosX = centerCirclePosX+leftLineAt_;


	sf::CircleShape kickoffCircle = sf::CircleShape(0.096354166*screenWidth_);
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
	ball_ = new Ball(startX, startY);
}

void Game::createGreen()
{
	int pos = centerLineAt_;
	int increment = screenWidth_/10;
	sf::Color lightGreen = sf::Color(50,200,50);
	sf::Color darkGreen = sf::Color(75,220,75);

	sf::RectangleShape rect(sf::Vector2f(increment, screenHeight_));

	for (int i = 0; i < 7; ++i)
	{
		rect.setPosition(pos+increment*i,0);
		if (i%2 == 0)
		{
			rect.setFillColor(darkGreen);
		}
		else 
		{
			rect.setFillColor(lightGreen);
		}
		field_.push_back(rect);
	}

	for (int i = 1; i < 6; ++i)
	{
		rect.setPosition(pos-increment*i,0);
		if (i%2 == 0)
		{
			rect.setFillColor(darkGreen);
		}
		else 
		{
			rect.setFillColor(lightGreen);
		}

		field_.push_back(rect);
	}
}


void Game::createGoals()
{
	double middleoflines = ((bottomLineAt_-topLineAt_)/2)+topLineAt_;
	double goalsStartAt = middleoflines - (0.2*(middleoflines));

	goalLeft_ = sf::RectangleShape(sf::Vector2f(0.03385416*screenWidth_,0.3*screenHeight_));
	goalLeft_.setPosition(0.01822916*screenWidth_,middleoflines-(0.15*screenHeight_));
	goalLeft_.setFillColor(sf::Color(0,0,0,0));
	goalLeft_.setOutlineThickness(5);
	goalLeft_.setOutlineColor(sf::Color(0,0,255));

	goalRight_ = sf::RectangleShape(sf::Vector2f(0.03385416*screenWidth_,0.3*screenHeight_));
	goalRight_.setPosition(rightLineAt_+5,middleoflines-(0.15*screenHeight_));
	goalRight_.setFillColor(sf::Color(0,0,0,0));
	goalRight_.setOutlineThickness(5);
	goalRight_.setOutlineColor(sf::Color(255,0,0));
}


void Game::createScoreLine()
{
	//sf::RectangleShape scoreLine_ = sf::RectangleShape(0,1200,1920,1350,sf::Color(0,0,0));
	scoreLine_ = sf::RectangleShape(sf::Vector2f(screenWidth_,0.1153846*screenHeight_));
	scoreLine_.setPosition(0,0.92307692307*screenHeight_);
	scoreLine_.setFillColor(sf::Color(0,0,0));

	//create text
	if (!font_.loadFromFile(resourcesPath_ + std::string("/font.ttf")))
	{
		std::cout << "[Game.cpp] Error loading font." << std::endl;
	}

	if (!scoreFont_.loadFromFile(resourcesPath_ + std::string("/blox.ttf")))
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
	if (!font_.loadFromFile(resourcesPath_ + std::string("/font.ttf")))
	{
		std::cout << "[Game.cpp] Error loading font." << std::endl;
	}

	fpsString_.setFont(font_);
	fpsString_.setCharacterSize(screenHeight_*0.03);

	fpsString_.move(0.85*screenWidth_,0.94*screenHeight_);
}


void Game::calculateLinePoistions()
{
	 leftLineAt_ = screenWidth_*0.0520833f;
	 topLineAt_ = screenHeight_*0.0192592f;
	 bottomLineAt_ = screenHeight_*0.86888f;
	 rightLineAt_ = screenWidth_*0.925312f;
	 centerLineAt_ = ((rightLineAt_-leftLineAt_)/2)+leftLineAt_;
}


void Game::initTime() 
{	
	if (!font_.loadFromFile(resourcesPath_ + std::string("/font.ttf")))
	{
		std::cout << "[Game.cpp] Error loading font." << std::endl;
	}

	time_.setFont(font_);
	time_.setCharacterSize(screenHeight_*0.06153846153);
	time_.move(0.05*screenWidth_,0.92307692307*screenHeight_);
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