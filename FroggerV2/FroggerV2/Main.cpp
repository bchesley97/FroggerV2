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

		game->getWindow()->clear(sf::Color::White);
		window_mutex.unlock();
		for (int i = 0; i < NUMBER_OF_LANES; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				traffic_mutex.lock();
				//sf::RectangleShape rect = ((*game->getTraffic())[i][j]->getShape());
				std::vector< Vehicle *> Vehicles = game->getTraffic().at(i);
				Vehicle *specificVehicle = Vehicles.at(j);
				sf::RectangleShape rectangle = *specificVehicle->getShape();

				window_mutex.lock();
				game->getWindow()->draw(rectangle); //draw all the traffic 
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
	sf::Clock clock;

	sf::Clock clocks[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES];

	while (1)
	{
		float delta_time[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES];

		for (int i = 0; i < NUMBER_OF_LANES; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				traffic_mutex.lock();
				car = ((game->getTraffic())[i][j]);
				float delta_time = clocks[i][j].restart().asMilliseconds();

				//position is the left most point I thinK????? CHECK
				if (car->getShape()->getPosition().x > (WINDOW_MAX_X))
				{
					//car->getShape()-> setOrigin(CAR_WIDTH_D2, car->getShape()->getPosition().y);
					car->getShape()->setPosition(CAR_WIDTH_D2, car->getShape()->getPosition().y);
				}

				else if (car->getShape()->getPosition().x + CAR_WIDTH < (WINDOW_MIN_X))
				{
					//car->getShape()->setOrigin(WINDOW_MAX_X - 2 * CAR_WIDTH_D2, car->getShape()->getPosition().y);
					car->getShape()->setPosition(WINDOW_MAX_X - 2 * CAR_WIDTH_D2, car->getShape()->getPosition().y);
				}
				else
				{
					car->getShape()->move(car->getSpeed() * delta_time / 100, 0);
				}
				traffic_mutex.unlock(); //release semaphore
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(75)); //NEEDS TO SLEEP LONGER THAN THE DRAW THREAD
	}

}

//check for collisions in this thread
void Update_Frog()
{

	bool movedHoriz = false;
	while (1)
	{
		endOfGame_mutex.lock();
		frog_mutex.lock();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			movedHoriz = true;
			game->setEndOfGame(game->detectLeftCollision());
			if (game->did_game_end())
			{
				break;
			}

			game->getFrog()->moveLeft();

			std::this_thread::sleep_for(std::chrono::milliseconds(70));

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			movedHoriz = true;
			game->setEndOfGame(game->detectRightCollision());
			if (game->did_game_end())
			{
				break;
			}
			game->getFrog()->moveRight();
			std::this_thread::sleep_for(std::chrono::milliseconds(70));


		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			game->setEndOfGame(game->detectUpCollision());
			if (game->did_game_end())
			{
				game->setEndOfGame(true);
				break;
			}
			game->getFrog()->moveUp();
			game->getFrog()->decrementLane();

			if (game->getFrog()->getLane() == 0)
			{
			break; //won the game 
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			game->setEndOfGame(game->detectBottomCollision());
			if (game->did_game_end())
			{
				break;
			}
			game->getFrog()->moveDown();
			game->getFrog()->incrementLane();

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (!movedHoriz && game->detectTrafficCollision())
		{
			game->setEndOfGame(true);
			break;
		}
		movedHoriz = false;
		frog_mutex.unlock();

		endOfGame_mutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	//std::thread endGame(End_Game);

}

//end of game thread, will display menu and redisplay choices
void End_Game()
{


}

