#include <SFML/Graphics.hpp>
#include <chrono>
#include <atomic>
#include <thread>
#include <mutex>
#include <SFML/OpenGL.hpp>
#include "car.h"
#include "Arena.h"
#include "Game.h"
/*************************** Thread Declarations *****************************************/
void StartGameThread();

void Draw_All_Objects();

void Update_Traffic();

void Update_Frog();

void frogDiedTransition();
/**************************** Global Variables *******************************/

Game *game;
sf::Text *livesText;
/******************************** semaphores *********************************/

int main() {

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
	livesText = new sf::Text("Lives: 3", font, CHAR_SIZE);
	livesText->setOrigin(0, 0);
	livesText->setPosition(10, LIVES_TEXT_PLACEMENT_Y);
	livesText->setFillColor(sf::Color::Black);

	/**************** initializations before start multithreaded application ******************/
	std::thread startGame(StartGameThread);

	startGame.join(); //to have main not run as a thread 

	//if game got here, means game is over and need to restart
	std::thread newStartGame(StartGameThread);
	newStartGame.detach();


}


void StartGameThread() {
	

		int difficulty = 2;
		//initialize traffic array
		game = new Game();
		game->setDifficulty(2); //default difficulty

		game->printWelcomeMenu();
		game->printDifficultyMenu();
		game->setDifficultyOfTraffic(); //update the difficulty that the player chose

		game = new Game();

		game->createWindow();
		game->getWindow()->setActive(false);
		game->setDifficultyOfTraffic();

		std::thread drawThread(Draw_All_Objects);
		std::thread updateThread(Update_Traffic);
		std::thread frogThread(Update_Frog);

		drawThread.join();
		updateThread.join();
		frogThread.join();
	
	while (1)
	{

		int choice;
		//game has ended, display appropriate menu's
		if (game->getWin())
		{
			//game was won, display menu
			choice = game->printWinnerScreen();
		}
		else
		{
			//game was lost
			choice = game->printLoserScreen();
		}

		if (choice == 1)
		{ 
			//menu
			game = new Game();

			game->printDifficultyMenu();
			game->setDifficultyOfTraffic(); //update the difficulty that the player chose
			difficulty = game->getDifficulty();
			game = new Game();
			game->createWindow();
			game->getWindow()->setActive(false);
			livesText->setString("Lives: 3");

			std::thread drawThread(Draw_All_Objects);
			std::thread updateThread(Update_Traffic);
			std::thread frogThread(Update_Frog);
			drawThread.join();
			updateThread.join();
			frogThread.join();
		}
		else if (choice == 2)
		{//replay
			game = new Game();
			game->createWindow();
			game->getWindow()->setActive(false);
			livesText->setString("Lives: 3");

			game->setDifficultyOfTraffic();
			std::thread drawThread(Draw_All_Objects);
			std::thread updateThread(Update_Traffic);
			std::thread frogThread(Update_Frog);
			drawThread.join();
			updateThread.join();
			frogThread.join();
		}
		else if (choice == 3)
		{//exit program
			break;
		}

	}
}

/**
will be responsible for drawing everything to the screen, no other thread should be updating the screen (for semaphore reasons)
**/

void Draw_All_Objects() {

	//open window

	game->getWindow()->setActive(true);
	game->getWindow()->setFramerateLimit(100); //60 fps


	sf::Text timeText;

	game->window_mutex.lock();
	int fade = game->getFrog()->getShape()->getFillColor().a;
	sf::Color newColor = game->getFrog()->getShape()->getFillColor();
	while (game->getWindow()->isOpen()) {
		
		game->window_mutex.unlock();
		
		sf::Event event;
		game->window_mutex.lock();
		game->getWindow()->setActive(true);
		while (game->getWindow()->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
			{
				game->getWindow()->close();
			}
		}
		game->window_mutex.unlock();

		game->updateScreen();
		//game->window_mutex.unlock();
		for (int i = 0; i < NUMBER_OF_LANES/2; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				game->window_mutex.lock();

				game->traffic_mutex.lock();
				//sf::RectangleShape rect = ((*game->getTraffic())[i][j]->getShape());
				std::vector< Vehicle *> Vehicles = game->getTraffic().getRoadTraffic()->at(i);
				Vehicle *specificVehicle = Vehicles.at(j);
				sf::RectangleShape rectangle = *specificVehicle->getShape();

				//game->window_mutex.lock();
				game->getWindow()->draw(rectangle); //draw all the traffic 
				game->getWindow()->draw(*game->getTraffic().getLogTraffic()->at(i).at(j)->getShape());
				
				
				game->traffic_mutex.unlock(); //release semaphore

				game->window_mutex.unlock();

			}
		}
		game->window_mutex.lock();

			//	game->window_mutex.unlock();

		//draw frog
		//game->window_mutex.lock();
		game->frog_mutex.lock();
		game->getWindow()->draw(*game->getFrog()->getShape());
		game->frog_mutex.unlock();

		game->getWindow()->draw(*livesText);
		game->getWindow()->display();

		game->window_mutex.unlock();
		if (game->didFrogDie())
		{

			if (fade >= 0)
			{
				game->frog_mutex.lock();
				newColor.a = newColor.a - 20;
				game->getFrog()->getShape()->setFillColor(newColor);
				game->getWindow()->draw(*game->getFrog()->getShape());
				game->getWindow()->draw(*livesText);
				fade = fade - 20;
				game->frog_mutex.unlock();
			}
			else
			{
				fade = game->getFrog()->getShape()->getFillColor().a;

				game->frog_mutex.lock();
				game->getFrog()->decrementLives();
				livesText->setString("Lives: " + std::to_string(game->getFrog()->getLives()));
				game->newFrog();
				game->getFrog()->reset();
				game->frog_mutex.unlock();
			}
		}
		if (game->did_game_end())
		{
			break;
		}

		

		//game->window_mutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(30));

		game->window_mutex.lock();
		game->getWindow()->setActive(true);

	}

	//game->window_mutex.unlock();
}

//update position of cars 

void Update_Traffic() {

	Vehicle *car;
	Log *log;
	sf::Clock clock;

	bool offScreen[(NUMBER_OF_LANES)][MAX_NUMBER_OF_VEHICLES] = { false }; //if cars went off the screen
	int waitTime[(NUMBER_OF_LANES)][MAX_NUMBER_OF_VEHICLES] = { 0 };

	while (1)
	{
		float delta_time[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES];
		

		for (int i = 0; i < NUMBER_OF_LANES/2; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				game->traffic_mutex.lock();

				car = ((game->getTraffic().getRoadTraffic())->at(i).at(j));
				float delta_time = game->clocks[i][j].restart().asMilliseconds();

				//position is the left most point I thinK????? CHECK
				if (car->getShape()->getPosition().x > (WINDOW_MAX_X))
				{
					offScreen[i][j] = true;
				}

				else if (car->getShape()->getPosition().x + CAR_WIDTH < (WINDOW_MIN_X))
				{
					offScreen[i][j] = true;
				}
				else
				{
					car->getShape()->move(car->getSpeed() * delta_time / 100, 0);
				}

				//wait about 1.4 seconds before respawning vehicle
				if (offScreen[i][j] && waitTime[i][j] == 40) //IMPLEMENT RANDOMMNESS FACTOR LATER
				{
					if (car->getSpeed() > 0)
					{
						car->getShape()->setPosition(0-CAR_WIDTH, car->getShape()->getPosition().y);
					}
					else
					{
						car->getShape()->setPosition(WINDOW_MAX_X, car->getShape()->getPosition().y);
					}
					waitTime[i][j] = 0; //reset wait time
					offScreen[i][j] = false;
				}
				else if (offScreen[i][j])
				{
					waitTime[i][j]++;
				}


				//update position of logs
				log = ((game->getTraffic().getLogTraffic())->at(i).at(j));
				delta_time = game->clocks[i + (int)(NUMBER_OF_LANES / 2)][j].restart().asMilliseconds();

				//reset clock for frog if its on a log
				if (i == game->getFrog()->getLane() && j == game->getFrog()->getLogLane())
				{
					game->frog_mutex.lock();
					if (i == game->getFrog()->getLane() && j == game->getFrog()->getLogLane())
						game->getFrog()->getShape()->move(log->getSpeed()*delta_time / 100, 0);

					game->frog_mutex.unlock();
				}



				if (log->getShape()->getPosition().x > (WINDOW_MAX_X))
				{
					offScreen[i + (int)(NUMBER_OF_LANES / 2)][j] = true;
				}

				else if (log->getShape()->getPosition().x + log->getShape()->getSize().x < (WINDOW_MIN_X))
				{
					offScreen[i + (int)(NUMBER_OF_LANES / 2)][j] = true;
				}
				else
				{
					log->getShape()->move(log->getSpeed() * delta_time / 100, 0);
				}

				//wait about 1.4 seconds before respawning vehicle
				if (offScreen[i + (int)(NUMBER_OF_LANES / 2)][j] && waitTime[i + (int)(NUMBER_OF_LANES / 2)][j] == 20) //IMPLEMENT RANDOMMNESS FACTOR LATER
				{
					if (log->getSpeed() > 0)
					{
						log->getShape()->setPosition(0 - log->getShape()->getSize().x, log->getShape()->getPosition().y);
					}
					else
					{
						log->getShape()->setPosition(WINDOW_MAX_X, log->getShape()->getPosition().y);
					}
					waitTime[i + (int)(NUMBER_OF_LANES / 2)][j] = 0; //reset wait time
					offScreen[i + (int)(NUMBER_OF_LANES / 2)][j] = false;
				}
				else if (offScreen[i + (int)(NUMBER_OF_LANES / 2)][j])
				{
					waitTime[i + (int)(NUMBER_OF_LANES / 2)][j]++;
				}

				game->traffic_mutex.unlock(); //release semaphore

			}
		}

		if (game->did_game_end())
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(60)); //NEEDS TO SLEEP LONGER THAN THE DRAW THREAD
	}

}

//check for collisions in this thread
void Update_Frog()
{
	bool onLog = false;
	bool movedHoriz = false;
	Log *movingLog = NULL;

	while (1)
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(60)); //NEEDS TO SLEEP LONGER THAN THE DRAW THREAD

		if (game->didFrogDie())
		{
			while (game->didFrogDie()); //wait in this thread if frog is dead
		}

		if (game->getFrog()->getLives() <= 0)
		{
			game->setEndOfGame(true);
			break; //game over, no  more lives 
		}

		game->endOfGame_mutex.lock();
		game->frog_mutex.lock();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			movedHoriz = true;
			if (game->getFrog()->getLane() >= (int)(NUMBER_OF_LANES / 2))
			{
				game->getFrog()->moveLeft();

				if (game->detectLeftCollision())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
					game->frogDied();
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;

					//break;
				}
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			
			}
			else
			{
				game->getFrog()->moveLeft();

				if (!game->moveOnLog(0))
				{
					//if jumped off of log
					game->frogDied();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;

				}
			//	game->getFrog()->moveLeft();

				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			}

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			movedHoriz = true;
			if (game->getFrog()->getLane() >= (int)(NUMBER_OF_LANES / 2))
			{
				game->getFrog()->moveRight();

				if (game->detectRightCollision())
				{
					game->frogDied();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
					continue;
				}
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			
			}
			else
			{
				game->getFrog()->moveRight();

				if (!game->moveOnLog(1))
				{
					game->frogDied();
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();

					continue;
				}

				//game->getFrog()->moveRight();
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			}


		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			game->getFrog()->moveUp();

			if (game->getFrog()->getLane() == 0)
			{
				if (game->jumpOnLilly())
				{
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(50)); 

					if (game->getNumFrogsOnLillies() == NUMBER_OF_LILLIES)
					{
						break; //game is over
					}
					else
					{
						game->getFrog()->reset();
						continue;
					}

				}

				else
				{
					game->frogDied();

					//missed the lilly
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;

				}

			}
			else if (game->getFrog()->getLane() > (int)(NUMBER_OF_LANES / 2))
			{

				if (game->detectUpCollision())
				{
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
					game->endOfGame_mutex.lock();
					game->frog_mutex.lock();
					game->frogDied();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;

				}

				game->getFrog()->decrementLane();
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			}

			//detect movement of frog onto logs
			else
			{
				if (game->jumpOnLog() == -1)
				{
					game->frogDied();
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;
				}
				game->getFrog()->decrementLane();
				onLog = true;

				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			}

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{

			game->getFrog()->moveDown();
			if (game->getFrog()->getLane() >= (int)(NUMBER_OF_LANES / 2))
			{
				if (game->getFrog()->getLane() == NUMBER_OF_LANES - 1)
				{
					//can always move back down 
					game->getFrog()->incrementLane();
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));

					continue;
				}

				if (game->getFrog()->getLane() == NUMBER_OF_LANES)
				{
					game->getFrog()->moveUp(); //DONT MOVE FROG OFF SCREEN
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;
				}

				if (game->detectBottomCollision())
				{
					game->frogDied();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
					game->endOfGame_mutex.lock();
					game->frog_mutex.lock();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;
				}
				game->getFrog()->incrementLane();
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));

				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			}

			//detect movement of frog onto logs
			else
			{
				if (game->jumpOffLog() == -1)
				{
					game->frogDied();

					//missed log
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;
				}
				game->getFrog()->incrementLane();
				onLog = true;

				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			}

		}
		if (!movedHoriz && game->detectTrafficCollision())
		{
			game->frogDied();

		}

		if (game->getFrog()->getShape()->getPosition().x >= WINDOW_MAX_X || game->getFrog()->getShape()->getPosition().x + FROG_SIZE <= WINDOW_MIN_X)
		{
			game->frogDied();
		}


		movedHoriz = false;

		game->frog_mutex.unlock();

		game->endOfGame_mutex.unlock();
	}

}