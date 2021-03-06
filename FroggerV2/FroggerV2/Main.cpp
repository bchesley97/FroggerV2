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

void Debug_Draw();
/**************************** Global Variables *******************************/

Game *game;

//extern Frog frog; //to be implemented later (by not me, hopefully)

//Vehicle **traffic[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES]; //array of all vehicles and lanes corresponding to those vehicles

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

	//open window

	game->getWindow()->setActive(true);
	game->getWindow()->setFramerateLimit(60); //60 fps
	while (game->getWindow()->isOpen()) {

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
				std::vector< Vehicle *> Vehicles = game->getTraffic()[i];
				Vehicle *specificVehicle = Vehicles.at(j);
				sf::RectangleShape rectangle = *specificVehicle->getShape();

				window_mutex.lock();
				game->getWindow()->draw(rectangle); //draw all the traffic 
				window_mutex.unlock();

				traffic_mutex.unlock(); //release semaphore
										//window.display();

			}

		}


		//draw frog
		frog_mutex.lock();
		game->getWindow()->draw(*game->getFrog()->getShape());
		frog_mutex.unlock();
		game->getWindow()->display();



		/*
		window.clear(BACK_COLOR);
		window.draw(sf::RectangleShape(sf::Vector2f(50, 50)));
		window.display();
		*/

		//	window.draw();

		//window.display();


		//car->setPosition(car->getPosition() + sf::Vector2f (car->getSpeed(), car->getSpeed()))
		//rectangle.setPosition(car->getCenterX(), car->getCenterY());


		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}


}

//update position of cars 

void Update_Traffic() {

	Vehicle *car;

	while (1)
	{
		for (int i = 0; i < NUMBER_OF_LANES; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				traffic_mutex.lock();
				car = ((game->getTraffic())[i][j]);
				//position is the left most point I thinK????? CHECK
				if (car->getShape()->getPosition().x >(WINDOW_MAX_X))
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
					car->getShape()->move(car->getSpeed(), 0);

				}
				traffic_mutex.unlock(); //release semaphore
			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(20)); //NEEDS TO SLEEP LONGER THAN THE DRAW THREAD
	}

}

//check for collisions in this thread
void Update_Frog()
{
	while (1)
	{
		endOfGame_mutex.lock();
		frog_mutex.lock();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			//game->setEndOfGame(game->detectLeftCollision());
			if (game->did_game_end())
			{
				break;
			}

			game->getFrog()->moveLeft();

			std::this_thread::sleep_for(std::chrono::milliseconds(70));

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{

			//game->setEndOfGame(game->detectRightCollision());
			if (game->did_game_end())
			{
				break;
			}
			game->getFrog()->moveRight();
			std::this_thread::sleep_for(std::chrono::milliseconds(70));


		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			//game->setEndOfGame(game->detectUpCollision());
			if (game->did_game_end())
			{
				break;
			}
			game->getFrog()->moveUp();
			game->getFrog()->decrementLane();

			std::this_thread::sleep_for(std::chrono::milliseconds(70));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			//game->setEndOfGame(game->detectBottomCollision());
			if (game->did_game_end())
			{
				break;
			}
			game->getFrog()->moveDown();
			game->getFrog()->incrementLane();

			std::this_thread::sleep_for(std::chrono::milliseconds(70));


		}


		frog_mutex.unlock();

		endOfGame_mutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	std::thread endGame(End_Game);

}

void End_Game()
{


}

void Debug_Draw()
{
	sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(5, 5));
	//shape.setPosition(750.f, 750.f);
	//shape.setOrigin(750.f, 750.f);
	shape->setFillColor(sf::Color::Black);

	sf::RenderWindow window(sf::VideoMode(WINDOW_MAX_X, WINDOW_MAX_Y),
		"Frogger",
		sf::Style::Default);
	shape->setPosition(100.f, 200.f);

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color::White);
		//shape->setPosition(0.f, 0.f);
		//window.draw(*shape);
		shape->move(5, 0);
		window.draw(*shape);
		window.display();

		std::this_thread::sleep_for(std::chrono::milliseconds(500)); //NEEDS TO SLEEP LONGER THAN THE DRAW THREAD


	}

}