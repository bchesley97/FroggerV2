/*
File: Threads.cpp
Purpose: holds all of the main threads and main routine for the Frogger game for the term project of COP3503.

PROJECT TEAM 28
*/



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
/*
Thread: StartGameThread
Purpose: this displays all menus and calls the other threads. This only runs when a game has ended (or first time started)
@param void
@return void
*/
void StartGameThread();

/*
Thread: Draw_All_Objects
Purpose: Draws all objects (traffic, frog, logs) to the screen. Only thread to have control of the window
@param: void
@return void
*/
void Draw_All_Objects();
/*
Thread: Update_Traffic
Purpose: Updates the position of all traffic (both road and log traffic).
@param void
@return void
*/
void Update_Traffic();

/*
Thread: Update_Frog
Purpose: Update the position of the frog based on keybaord presses
@param void
@return void
*/
void Update_Frog();
/**************************** Global Variables *******************************/

Game *game; //game pointer that is the main data structure for the frogger application
sf::Text *livesText; //text for the lives remaining 

/*
Function: main
Purpose: Gotta have it. Starts all threads and initializes the global livesText variable. 

*/
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
	std::thread newStartGame(StartGameThread);
	newStartGame.detach();
}

/*
Thread: StartGameThread
Purpose: this displays all menus and calls the other threads. This only runs when a game has ended (or first time started)
@param void
@return void
*/
void StartGameThread() {
	

		int difficulty = 2;
		//initialize traffic array
		game = new Game();
		game->setDifficulty(2); //default difficulty

		game->printWelcomeMenu();
		game->printDifficultyMenu();
		game->setDifficultyOfTraffic(); //update the difficulty that the player chose

		game = new Game(); //create new game

		game->createWindow(); //create window 
		game->getWindow()->setActive(false);
		game->setDifficultyOfTraffic();

		std::thread drawThread(Draw_All_Objects); //create threads for the first time 
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

			std::thread drawThread(Draw_All_Objects); //reset game 
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
			std::thread updateThread(Update_Traffic); //reset game 
			std::thread frogThread(Update_Frog);
			drawThread.join();
			updateThread.join();
			frogThread.join();
		}
		else if (choice == 3)
		{
			//exit program
			break;
		}

	}
}

/*
Thread: Draw_All_Objects
Purpose: Draws all objects (traffic, frog, logs) to the screen. Only thread to have control of the window
@param: void
@return void
*/
void Draw_All_Objects() {

	//open window

	game->getWindow()->setActive(true);
	game->getWindow()->setFramerateLimit(100); //60 fps


	sf::Text timeText;

	game->window_mutex.lock();
	int fade = game->getFrog()->getShape()->getFillColor().a; //initialize fade variable when a frog dies
	sf::Color newColor = game->getFrog()->getShape()->getFillColor();
	
	while (game->getWindow()->isOpen()) {
		
		game->window_mutex.unlock();
		
		sf::Event event;
		game->window_mutex.lock(); //lock mutex
		game->getWindow()->setActive(true);
		while (game->getWindow()->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
			{
				game->getWindow()->close();
			}
		}
		game->window_mutex.unlock();

		game->updateScreen();

		for (int i = 0; i < NUMBER_OF_LANES/2; i++)
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				game->window_mutex.lock();

				game->traffic_mutex.lock();

				std::vector< Vehicle *> Vehicles = game->getTraffic().getRoadTraffic()->at(i);
				Vehicle *specificVehicle = Vehicles.at(j);
				sf::RectangleShape rectangle = *specificVehicle->getShape();

				game->getWindow()->draw(rectangle); //draw all the traffic 
				game->getWindow()->draw(*game->getTraffic().getLogTraffic()->at(i).at(j)->getShape());
				
				
				game->traffic_mutex.unlock(); //release semaphore

				game->window_mutex.unlock();

			}
		}
		game->window_mutex.lock();

		//draw frog
		game->frog_mutex.lock();
		game->getWindow()->draw(*game->getFrog()->getShape()); //draw frog
		game->frog_mutex.unlock();

		game->getWindow()->draw(*livesText); //draw livesText 
		game->getWindow()->display();

		game->window_mutex.unlock();
		if (game->didFrogDie()) //used to fade the frog out when the frog diess
		{
			if (fade >= 0)
			{
				game->frog_mutex.lock(); //fade the frog
				if (fade <= 0)
				{
					newColor.a = 0;
				}
				else
				{
					newColor.a = newColor.a - 15;
				}
				game->getFrog()->getShape()->setFillColor(newColor); 
				game->getWindow()->draw(*game->getFrog()->getShape());
				game->getWindow()->draw(*livesText);
				fade = fade - 15;
				game->frog_mutex.unlock();
			}
			else
			{
				game->frog_mutex.lock(); //fade is over, reset frog variables
				game->getFrog()->decrementLives();
				livesText->setString("Lives: " + std::to_string(game->getFrog()->getLives()));
				game->newFrog();
				game->getFrog()->reset();
				game->getFrog()->getShape()->setFillColor(sf::Color::Green); //reset fill color
				fade = game->getFrog()->getShape()->getFillColor().a;

				game->frog_mutex.unlock();
			}
		}
		if (game->did_game_end())
		{
			break; //if game is over, break from the thread
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(30)); //sleep

		game->window_mutex.lock();
		game->getWindow()->setActive(true);

	}

	//end of thread, only gets here after a game ends
}

/*
Thread: Update_Traffic
Purpose: Updates the position of all traffic (both road and log traffic).
@param void
@return void
*/

void Update_Traffic() {

	Vehicle *car;
	Log *log;
	sf::Clock clock;

	bool offScreen[(NUMBER_OF_LANES)][MAX_NUMBER_OF_VEHICLES] = { false }; //if cars went off the screen
	int waitTime[(NUMBER_OF_LANES)][MAX_NUMBER_OF_VEHICLES] = { 0 }; //use wait time and offscreen to determine when to reset traffic when it goes off the screen

	while (1)
	{
		float delta_time[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES]; //used to provide a consisten movement of traffic on the screen

		for (int i = 0; i < NUMBER_OF_LANES / 2; i++) //parse through all vehicles and update their positions based on their speed
		{
			for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
			{
				game->traffic_mutex.lock();

				car = ((game->getTraffic().getRoadTraffic())->at(i).at(j));
				float delta_time = game->clocks[i][j].restart().asMilliseconds();

				if (car->getShape()->getPosition().x > (WINDOW_MAX_X)) //set variables if off the screen
				{
					offScreen[i][j] = true;
				}

				else if (car->getShape()->getPosition().x + CAR_WIDTH < (WINDOW_MIN_X)) //set variables if off the screen
				{
					offScreen[i][j] = true;
				}
				else
				{
					car->getShape()->move(car->getSpeed() * delta_time / 100, 0); //move the car
				}

				//wait about 1.4 seconds before respawning vehicle
				if (offScreen[i][j] && waitTime[i][j] == 40)
				{
					if (car->getSpeed() > 0)
					{
						car->getShape()->setPosition(0 - CAR_WIDTH, car->getShape()->getPosition().y); //reset position of cars
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

				//same logic for logs as for traffuc 
				//update position of logs
				log = ((game->getTraffic().getLogTraffic())->at(i).at(j));
				delta_time = game->clocks[i + (int)(NUMBER_OF_LANES / 2)][j].restart().asMilliseconds();

				//reset clock for frog if its on a log
				if (i == game->getFrog()->getLane() && j == game->getFrog()->getLogLane() && !game->didFrogDie()) //if a frog is on a log, update position accordingly
				{
					game->frog_mutex.lock();
					if (i == game->getFrog()->getLane() && j == game->getFrog()->getLogLane())
						game->getFrog()->getShape()->move(log->getSpeed()*delta_time / 100, 0);

					game->frog_mutex.unlock();
				}

				if (log->getShape()->getPosition().x > (WINDOW_MAX_X))
				{
					offScreen[i + (int)(NUMBER_OF_LANES / 2)][j] = true; //update position if off the screen
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
						log->getShape()->setPosition(WINDOW_MAX_X, log->getShape()->getPosition().y); //update position based on sign of speed
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
			break; //break from thread if game is over
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(60)); //NEEDS TO SLEEP LONGER THAN THE DRAW THREAD
	}

}

/*
Thread: Update_Frog
Purpose: Update the position of the frog based on keybaord presses
@param void
@return void
*/
void Update_Frog()
{
	bool onLog = false; //used to determine if a frog jumped on a log
	bool movedHoriz = false; //used to determine if the left or right keys have been pressed

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //go left if left key is pressed
		{
			movedHoriz = true;
			//check for traffic collisions based on lane
			if (game->getFrog()->getLane() >= (int)(NUMBER_OF_LANES / 2))
			{
				game->getFrog()->moveLeft(); 

				if (game->detectLeftCollision())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
					game->frogDied(); //if collision detected, kill the frog
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;

					//break;
				}
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock(); //if no collision, move the frog 
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			
			}
			else
			{
				game->getFrog()->moveLeft();

				if (!game->moveOnLog(0))
				{
					game->getFrog()->setLogLane(-1);
					//if jumped off of log
					game->frogDied();

					game->frog_mutex.unlock(); 
					game->endOfGame_mutex.unlock();
					continue;

				}

				game->frog_mutex.unlock(); //sleep 
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

				if (game->detectRightCollision()) //if collided to the right, then kill the frog
				{
					game->frogDied();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
					continue;
				}
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock(); //otherwise just sleep for a debounce time
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			
			}
			else
			{
				//on a log, detect valid movements
				game->getFrog()->moveRight();

				if (!game->moveOnLog(1))
				{
					game->getFrog()->setLogLane(-1);
					game->frogDied(); //detect if valid move on a log
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();

					continue;
				}
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
				if (game->jumpOnLilly()) //detect if jumped on a lilly
				{
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					game->getFrog()->setLogLane(-1);


					if (game->getNumFrogsOnLillies() == NUMBER_OF_LILLIES)
					{
						break; //game is over
					}
					else
					{
						game->getFrog()->reset(); //reset frog to original position
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
			else if (game->getFrog()->getLane() > (int)(NUMBER_OF_LANES / 2)) //detect collisions between traffic
			{

				if (game->detectUpCollision())
				{
					game->frog_mutex.unlock(); //if collision detected, frog died
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					game->endOfGame_mutex.lock();
					game->frog_mutex.lock();
					game->frogDied();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;

				}

				game->getFrog()->decrementLane(); //decrement the lane
				game->frog_mutex.unlock();
				game->endOfGame_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
				game->endOfGame_mutex.lock();
				game->frog_mutex.lock();
			}

			//detect movement of frog onto logs
			else
			{
				if (game->jumpOnLog() == -1)
				{
					game->getFrog()->setLogLane(-1);

					game->frogDied(); //missed the log, kill frog 
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
			if (game->getFrog()->getLane() >= (int)(NUMBER_OF_LANES / 2)) //detect collisions betweent frog and road traffic
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
					game->frogDied(); //collision with traffic 

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
					game->endOfGame_mutex.lock();
					game->frog_mutex.lock();

					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;
				}
				game->getFrog()->incrementLane(); //increment the lane
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
					game->getFrog()->setLogLane(-1);

					game->frogDied();

					//missed log, kill frog
					game->frog_mutex.unlock();
					game->endOfGame_mutex.unlock();
					continue;
				}
				game->getFrog()->incrementLane(); //set boolean flag and increment lane
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
			game->frogDied(); //if collision between none moving frog and traffic, kill frog

		}

		if (game->getFrog()->getShape()->getPosition().x >= WINDOW_MAX_X || game->getFrog()->getShape()->getPosition().x + FROG_SIZE <= WINDOW_MIN_X)
		{
			game->frogDied(); //if off screen, frog died
		}


		movedHoriz = false; //reset boolean flag

		game->frog_mutex.unlock();

		game->endOfGame_mutex.unlock();
	}

}