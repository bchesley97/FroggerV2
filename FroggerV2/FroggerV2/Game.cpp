#include "Game.h"
#include "Frog.h"

Game::Game()
{

	//initialize traffic vector
	for (int i = 0; i<NUMBER_OF_LANES; i++)
	{
		traffic.push_back(std::vector<Vehicle*>());
		int speed = (rand() % 2);
		int xPosition = 0;
		//determine whether to make speed negative (and thus start on the right side of the screen
		if (speed)
		{
			speed = -((rand() % MAX_VEHICLE_SPEED) + 1);

		}
		else
		{
			speed = (rand() % MAX_VEHICLE_SPEED) + 1;

		}

		for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
		{
			int differential = (rand() % FROG_SIZE) + 4;
			traffic[i].push_back(new Car(j*FROG_SIZE+differential + j*(FROG_SIZE+differential), (i+1)*FROG_SIZE, speed, sf::Color::Black)); 
			traffic[i][j]->setLane(i);
			traffic[i][j]->setSpeed(speed);
		}
	}

	frog = new Frog(); //create new frog 

					   //create window
	window = new sf::RenderWindow(sf::VideoMode(WINDOW_MAX_X, WINDOW_MAX_Y),
		"Frogger",
		sf::Style::Default);
}


sf::RenderWindow* Game::getWindow()
{
	return window;
}



std::vector<std::vector<Vehicle*> > Game::getTraffic()
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

//helper function to detect intersections between traffic and frog
bool intersects(sf::RectangleShape rect1, sf::RectangleShape rect2)
{
	sf::FloatRect shape1 = rect1.getGlobalBounds();
	sf::FloatRect shape2 = rect2.getGlobalBounds();

	return (shape1.intersects(shape2) || shape2.intersects(shape1));
}

//THESE COLLISION FUNCTIONS ARE FOR WHEN THE FROG MOVES

bool Game::detectLeftCollision()
{
	if (frog->getLane() == NUMBER_OF_LANES  || frog->getLane() == 0)
		return false;

	//need to create new temporary frog to update the position based on the vertical jump, then detect collisions with the new moved frog
	// this is because the function SHOULD BE CALLED BEFORE THE FROG IS ACTUALLY MOVED 
	sf::RectangleShape tempFrog(sf::Vector2f(FROG_SIZE, FROG_SIZE));
	tempFrog.setPosition(frog->getShape()->getPosition().x - frog->getHorizJump(), frog->getShape()->getPosition().y);

	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{

		if (intersects(tempFrog, *traffic.at(frog->getLane()).at(j)->getShape()))
		{
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
	tempFrog.setPosition(frog->getShape()->getPosition().x + frog->getHorizJump(), frog->getShape()->getPosition().y);

	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{

		if (intersects(tempFrog, *traffic.at(frog->getLane()).at(j)->getShape()))
		{
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
	tempFrog.setPosition(frog->getShape()->getPosition().x, frog->getShape()->getPosition().y - frog->getJump());

	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{
	
		if (intersects(tempFrog, *traffic.at(frog->getLane() - 1).at(j)->getShape()))
		{
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
	tempFrog.setPosition(frog->getShape()->getPosition().x, frog->getShape()->getPosition().y + frog->getJump());

	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{

		if (intersects(tempFrog, *traffic.at(frog->getLane() + 1).at(j)->getShape()))
		{
			return true;
		}

	}

	return false;
}

//THESE COLLISION FUNCTIONS ARE FOR THE TRAFFIC, IE WHEN THE TRAFFIC IS MOVING

bool Game::detectTrafficCollision()
{
	if (frog->getLane() == NUMBER_OF_LANES || frog->getLane() == 0)
		return false;

	//only need to check if the traffic collided in the current lane of the frog
	for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
	{
		if (intersects(*frog->getShape(), *traffic.at(frog->getLane()).at(j)->getShape()))
			return true;
	}

	return false;

}


