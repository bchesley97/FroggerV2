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

void End_Game();
/**************************** Global Variables *******************************/

Game *game;

//extern Frog frog; //to be implemented later (by not me, hopefully)


std::mutex traffic_mutex; //mutex semaphore for traffic array 

std::mutex frog_mutex; //mutex semaphore for frog (might need)

std::mutex endOfGame_mutex;

std::mutex window_mutex;
/******************************** semaphores *********************************/


int main() {

	/**************** initializations before start multithreaded application ******************/

	//std::thread testThread(thread1 );

	std::thread startGame(StartGameThread);

	startGame.join(); //to have main not run as a thread 
					  //	return 0;


	//if game got here, means game is over and need to restart
	std::thread newStartGame(StartGameThread);
	newStartGame.detach();


}


void StartGameThread() {

	//DO INITIALIZATIONS HERE 	
	//create seed for random number generator

	//initialize traffic array

	game = new Game();

	game->getWindow()->setActive(false);
	//std::thread updateThread(Update_Traffic);
	std::thread drawThread(Draw_All_Objects);

	//	updateThread.join();
	drawThread.join();

	//std::thread testDraw(Debug_Draw);
	//testDraw.join();
}

/**
will be responsible for drawing everything to the screen, no other thread should be updating the screen (for semaphore reasons)
**/

void Draw_All_Objects() {


	std::thread updateThread(Update_Traffic);
	std::thread frogThread(Update_Frog);


	//detach?? Not sure if works properly

	//open window

	game->getWindow()->setActive(true);
	game->getWindow()->setFramerateLimit(100); //60 fps

	window_mutex.lock();
	while (game->getWindow()->isOpen()) {
		
		window_mutex.unlock();
		
		sf::Event event;
		window_mutex.lock();
		while (game->getWindow()->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
			{
				game->getWindow()->close();
			}
		}

		game->updateScreen();
		window_mutex.unlock();
		for (int i = 0; i < NUMBER_OF_LANES/2; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				traffic_mutex.lock();
				//sf::RectangleShape rect = ((*game->getTraffic())[i][j]->getShape());
				std::vector< Vehicle *> Vehicles = game->getTraffic().getRoadTraffic()->at(i);
				Vehicle *specificVehicle = Vehicles.at(j);
				sf::RectangleShape rectangle = *specificVehicle->getShape();

				window_mutex.lock();
				game->getWindow()->draw(rectangle); //draw all the traffic 
				game->getWindow()->draw(*game->getTraffic().getLogTraffic()->at(i).at(j)->getShape());
				
				window_mutex.unlock();
				
				traffic_mutex.unlock(); //release semaphore
			}
		}

		//draw frog
		window_mutex.lock();
		frog_mutex.lock();
		game->getWindow()->draw(*game->getFrog()->getShape());
		frog_mutex.unlock();
		game->getWindow()->display();
		window_mutex.unlock();


		std::this_thread::sleep_for(std::chrono::milliseconds(30));

		window_mutex.lock();
	}


}

//update position of cars 

void Update_Traffic() {

	Vehicle *car;
	Log *log;
	sf::Clock clock;

	sf::Clock clocks[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES];
	bool offScreen[(NUMBER_OF_LANES)][MAX_NUMBER_OF_VEHICLES] = { false }; //if cars went off the screen
	int waitTime[(NUMBER_OF_LANES)][MAX_NUMBER_OF_VEHICLES] = { 0 };

	while (1)
	{
		float delta_time[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES];
		

		for (int i = 0; i < NUMBER_OF_LANES/2; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				traffic_mutex.lock();

				car = ((game->getTraffic().getRoadTraffic())->at(i).at(j));
				float delta_time = clocks[i][j].restart().asMilliseconds();

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
				delta_time = clocks[i + (int)(NUMBER_OF_LANES / 2)][j].restart().asMilliseconds();

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
					frog_mutex.lock();
					if (i == game->getFrog()->getLane() && j == game->getFrog()->getLogLane())
						game->getFrog()->getShape()->move(log->getSpeed()*delta_time/100, 0);
						
					frog_mutex.unlock();
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

				traffic_mutex.unlock(); //release semaphore

			}
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
		endOfGame_mutex.lock();
		frog_mutex.lock();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			movedHoriz = true;
			if (game->getFrog()->getLane() >= (int)(NUMBER_OF_LANES / 2))
			{
				game->getFrog()->moveLeft();

				//game->setEndOfGame(game->detectRightCollision());
				if (game->did_game_end())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(100));

					break;
				}
				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				endOfGame_mutex.lock();
				frog_mutex.lock();
			
			}
			else
			{
				traffic_mutex.lock();
				if (!game->moveOnLog(0))
				{
					game->setEndOfGame(true); //missed the log
					break;
				}
				traffic_mutex.unlock();
				game->getFrog()->moveLeft();
				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				endOfGame_mutex.lock();
				frog_mutex.lock();
			}

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			movedHoriz = true;
			if (game->getFrog()->getLane() >= (int)(NUMBER_OF_LANES / 2))
			{
				game->getFrog()->moveRight();

				//game->setEndOfGame(game->detectRightCollision());
				if (game->did_game_end())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(300));

					break;
				}
				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				endOfGame_mutex.lock();
				frog_mutex.lock();
			
			}
			else
			{
				traffic_mutex.lock();
				if (!game->moveOnLog(1))
				{
					game->setEndOfGame(true); //missed the log
					break;
				}
				traffic_mutex.unlock();

				game->getFrog()->moveRight();
				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				endOfGame_mutex.lock();
				frog_mutex.lock();
			}


		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{

			game->getFrog()->moveUp();
			if (game->getFrog()->getLane() > (int)(NUMBER_OF_LANES / 2))
			{

				game->setEndOfGame(game->detectUpCollision());
				if (game->did_game_end())
				{
					frog_mutex.unlock();
					endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					endOfGame_mutex.lock();
					frog_mutex.lock();

					game->setEndOfGame(true);
					break;
				}
				game->getFrog()->decrementLane();
				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
				endOfGame_mutex.lock();
				frog_mutex.lock();
			}

			//detect movement of frog onto logs
			else
			{
				traffic_mutex.lock();
				if (game->jumpOnLog() == -1)
				{
					game->setEndOfGame(true); //missed the log
					break;
				}
				traffic_mutex.unlock();
				game->getFrog()->decrementLane();
				onLog = true;
				
				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				endOfGame_mutex.lock();
				frog_mutex.lock();
			}

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{

			game->getFrog()->moveDown();
			if (game->getFrog()->getLane() > (int)(NUMBER_OF_LANES / 2))
			{

				game->setEndOfGame(game->detectBottomCollision());
				if (game->did_game_end())
				{
					frog_mutex.unlock();
					endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					endOfGame_mutex.lock();
					frog_mutex.lock();

					game->setEndOfGame(true);
					break;
				}
				game->getFrog()->incrementLane();
				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

				endOfGame_mutex.lock();
				frog_mutex.lock();
			}

			//detect movement of frog onto logs
			else
			{
				traffic_mutex.lock();
				if (game->jumpOffLog() == -1)
				{
					game->setEndOfGame(true); //missed the log
					break;
				}
				traffic_mutex.unlock();
				game->getFrog()->incrementLane();
				onLog = true;

				frog_mutex.unlock();
				endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				endOfGame_mutex.lock();
				frog_mutex.lock();
			}

		}
		//if (!movedHoriz && game->detectTrafficCollision())
		if(0)
		{
			frog_mutex.unlock();
			endOfGame_mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			endOfGame_mutex.lock();
				game->setEndOfGame(true);
			frog_mutex.lock();		
			break;
		}
		movedHoriz = false;

		frog_mutex.unlock();

		endOfGame_mutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	//std::thread endGame(End_Game);

}

//end of game thread, will display menu and redisplay choices
void End_Game()
{


}

