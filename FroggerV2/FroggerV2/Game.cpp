#include "Game.h"
#include "Frog.h"
#include "Traffic.h"

Game::Game()
{

	//initialize traffic vector
	traffic = *new Traffic();

	frog = new Frog(); //create new frog 

	//create new rectagngles for arena 
	road = new sf::RectangleShape(sf::Vector2f(WINDOW_MAX_X, WINDOW_MAX_Y/2));
	road->setPosition(sf::Vector2f(0, WINDOW_MAX_Y / 2)); //at the bottom half of the screen
	road->setFillColor(sf::Color::White);


	water = new sf::RectangleShape(sf::Vector2f(WINDOW_MAX_X, WINDOW_MAX_Y / 2));
	water->setPosition(0, 0); //at the top of the screen
	water->setFillColor(sf::Color::Blue); //water is usually blue (in the U.S., anyways) 

	//create lillies (land on to win game)
	for (int i = 0; i < NUMBER_OF_LILLIES; i++)
	{
		lillies.push_back(*new Lilly());
		lillies.at(i).getShape()->setPosition(i * 2 * LILY_PAD_WIDTH, 0); //at top of arena
	}

	win = false;
	/*				   //create window
	window = new sf::RenderWindow(	sf::VideoMode(WINDOW_MAX_X, WINDOW_MAX_Y),
									"Frogger",
									sf::Style::Default);
	*/
}


sf::RenderWindow* Game::getWindow()
{
	return window;
}

void Game::createWindow()
{
	window = new sf::RenderWindow(sf::VideoMode(WINDOW_MAX_X, WINDOW_MAX_Y),
		"Frogger",
		sf::Style::Default);
}

Traffic Game::getTraffic()
{
	return traffic;

}

Frog* Game::getFrog()
{
	return frog;
}

bool Game::did_game_end()
{
	return endOfGame;
}
void Game::setEndOfGame(bool endOfGame)
{
	this->endOfGame = endOfGame;
}

float Game::getTime()
{
	return time;
}

void Game::setTime(float time)
{
	this->time = time;
}
void Game::setDifficulty(int difficulty)
{
	this->difficulty = difficulty;
}
int Game::getDifficulty()
{
	return difficulty;
}
std::vector<Lilly> *Game:: getLillies()
{
	return &lillies;
}
//this will redraw the arena, use it as a window.clear()
void Game::updateScreen()
{
	this->window->clear(sf::Color::White);
	this->window->draw(*water); //redraw arena
	this->window->draw(*road);
	
	for (int i = 0; i < NUMBER_OF_LILLIES; i++)
	{
		this->window->draw(*lillies.at(i).getShape());
	}

}
bool Game::getWin()
{
	return win;
}
int Game::getNumFrogsOnLillies()
{
	return numFrogsOnLillies;
}
void Game::incrementNumFrogsOnLillies()
{
	++numFrogsOnLillies;
}
void Game::restartArena()
{
	frog = new Frog(); //create new frog 


	traffic_mutex.try_lock();
	frog_mutex.try_lock();
	endOfGame_mutex.try_lock();
	window_mutex.try_lock();

	traffic_mutex.unlock();
	frog_mutex.unlock();
	endOfGame_mutex.unlock();
	window_mutex.unlock();


}
//helper function to detect intersections between traffic and frog
bool intersects(sf::RectangleShape rect1, sf::RectangleShape rect2)
{
	sf::FloatRect shape1 = rect1.getGlobalBounds();
	sf::FloatRect shape2 = rect2.getGlobalBounds();

	return (shape1.intersects(shape2) || shape2.intersects(shape1));
}

//THESE COLLISION FUNCTIONS ARE FOR WHEN THE FROG MOVES FOR THE TRAFFIC SIDE OF ARENA

bool Game::detectLeftCollision()
{
	if (frog->getLane() == NUMBER_OF_LANES  || frog->getLane() == 0)
		return false;

	//need to create new temporary frog to update the position based on the vertical jump, then detect collisions with the new moved frog
	// this is because the function SHOULD BE CALLED BEFORE THE FROG IS ACTUALLY MOVED 
	sf::RectangleShape tempFrog(sf::Vector2f(FROG_SIZE, FROG_SIZE));
	tempFrog.setPosition(frog->getShape()->getPosition().x, frog->getShape()->getPosition().y);
	int newLane = frog->getLane() - NUMBER_OF_LANES / 2 ;

	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{

		if (intersects(tempFrog, *traffic.getRoadTraffic()->at(newLane).at(j)->getShape()))
		{
			frog->decrementLives();
			return true;
		}

	}

	return false;

}
bool Game::detectRightCollision()
{
	if (frog->getLane() == NUMBER_OF_LANES || frog->getLane() == 0)
		return false;

	//need to create new temporary frog to update the position based on the vertical jump, then detect collisions with the new moved frog
	// this is because the function SHOULD BE CALLED BEFORE THE FROG IS ACTUALLY MOVED 
	sf::RectangleShape tempFrog(sf::Vector2f(FROG_SIZE, FROG_SIZE));
	tempFrog.setPosition(frog->getShape()->getPosition().x, frog->getShape()->getPosition().y);
	int newLane = frog->getLane() - NUMBER_OF_LANES / 2 ;


	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{

		if (intersects(tempFrog, *traffic.getRoadTraffic()->at(newLane).at(j)->getShape()))
		{
			frog->decrementLives();

			return true;
		}

	}

	return false;

}
bool Game::detectUpCollision()
{
	//need to create new temporary frog to update the position based on the vertical jump, then detect collisions with the new moved frog
	// this is because the function SHOULD BE CALLED BEFORE THE FROG IS ACTUALLY MOVED 
	sf::RectangleShape tempFrog(sf::Vector2f(FROG_SIZE, FROG_SIZE));
	tempFrog.setPosition(frog->getShape()->getPosition().x, frog->getShape()->getPosition().y);

	int newLane = frog->getLane() - NUMBER_OF_LANES/2 -1;

	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{
	
		if (intersects(tempFrog, *traffic.getRoadTraffic()->at(newLane).at(j)->getShape()))
		{
			frog->decrementLives();

			return true;
		}

	}
	
	return false;
}
bool Game::detectBottomCollision()
{
	//need to create new temporary frog to update the position based on the vertical jump, then detect collisions with the new moved frog
	// this is because the function SHOULD BE CALLED BEFORE THE FROG IS ACTUALLY MOVED 
	sf::RectangleShape tempFrog(sf::Vector2f(FROG_SIZE, FROG_SIZE));
	tempFrog.setPosition(frog->getShape()->getPosition().x, frog->getShape()->getPosition().y);
	int newLane = frog->getLane() - NUMBER_OF_LANES / 2 + 1;

	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{
		if (intersects(tempFrog, *traffic.getRoadTraffic()->at(newLane).at(j)->getShape()))
		{
			frog->decrementLives();

			return true;
		}

	}

	return false;
}

//THESE COLLISION FUNCTIONS ARE FOR THE TRAFFIC, IE WHEN THE TRAFFIC IS MOVING

bool Game::detectTrafficCollision()
{
	if (frog->getLane() == NUMBER_OF_LANES || frog->getLane() < NUMBER_OF_LANES/2)
		return false;

	int newLane = frog->getLane() - NUMBER_OF_LANES / 2 ;
	//only need to check if the traffic collided in the current lane of the frog
	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{
		if (intersects(*frog->getShape(), *traffic.getRoadTraffic()->at(newLane).at(j)->getShape()))
		{
			frog->decrementLives();
			return true;

		}
	}

	return false;

}

int Game::jumpOnLog()
{
	int newLane = frog->getLane()-1;
	int wiggleRoom = 2*traffic.getLogTraffic()->at(0).at(0)->getShape()->getEdgeRadius();
	//only need to check if the traffic collided in the current lane of the frog
	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{
		if (traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x - wiggleRoom <= frog->getShape()->getPosition().x
			&& traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x + traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getSize().x + wiggleRoom >= frog->getShape()->getPosition().x + FROG_SIZE)
		{
			frog->setLogLane(j);
			return j;
		}

	}
	frog->decrementLives();

	return -1;
}

int Game::jumpOffLog()
{
	int newLane = frog->getLane() + 1;
	if (newLane >= (int)(NUMBER_OF_LANES / 2))
	{
		return true; //jump back to land is always valid
	}
	int wiggleRoom = 2 * traffic.getLogTraffic()->at(0).at(0)->getShape()->getEdgeRadius();
	//only need to check if the traffic collided in the current lane of the frog
	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{
		if (traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x - wiggleRoom <= frog->getShape()->getPosition().x
			&& traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x + traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getSize().x + wiggleRoom >= frog->getShape()->getPosition().x + FROG_SIZE)
		{
			frog->setLogLane(j);
			return j;
		}
	}
	frog->decrementLives();

	return -1;
}


//returns true if valid movement
//left = 0, right = 1
bool Game::moveOnLog(bool right)
{
	int newLane = frog->getLane();
	int wiggleRoom = 2 * traffic.getLogTraffic()->at(0).at(0)->getShape()->getEdgeRadius();
	//only need to check if the traffic collided in the current lane of the frog

	if (right)
	{
		for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
		{
			if (traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x - wiggleRoom <= frog->getShape()->getPosition().x + frog->getHorizJump()
				&& traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x + traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getSize().x + wiggleRoom >= frog->getShape()->getPosition().x + FROG_SIZE + frog->getHorizJump())
			{
				return true;
			}
		}
		frog->decrementLives();

		return false;
	}
	else 
	{
		for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
		{
			if (traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x - wiggleRoom <= frog->getShape()->getPosition().x - frog->getHorizJump()
				&& traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getPosition().x + traffic.getLogTraffic()->at(newLane).at(j)->getShape()->getSize().x + wiggleRoom >= frog->getShape()->getPosition().x + FROG_SIZE - frog->getHorizJump())
			{
				return true;
			}
		}
		frog->decrementLives();

		return false;
	}


}


bool Game::jumpOnLilly()
{
	int wiggleRoom =  FROG_SIZE/4;


	for (int i = 0; i < NUMBER_OF_LILLIES; i++)
	{
		if (lillies.at(i).getShape()->getPosition().x - wiggleRoom <= frog->getShape()->getPosition().x
			&& lillies.at(i).getShape()->getPosition().x + lillies.at(i).getShape()->getSize().x + wiggleRoom >= frog->getShape()->getPosition().x + FROG_SIZE)
		{
			//win = true;
			//frog->getShape()->setPosition(lillies.at(i).getShape()->getPosition().x, 0);
			lillies.at(i).getShape()->setFillColor(sf::Color::Green);

			//check if game is over
			if (numFrogsOnLillies == NUMBER_OF_LILLIES)
			{
				endOfGame = true;
				win = true; //won the game if all frogs on lillies

			}
			return true; //game has been won
		}

	}
	frog->decrementLives();
	win = false;
	return false;

}
