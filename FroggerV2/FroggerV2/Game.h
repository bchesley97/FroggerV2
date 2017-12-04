/*
File: Game.h
Purpose: Holds member variables and class definition for the Game. The Game variable is the main point of contact between the 
		user and the UI. This holds all facets of the game, including the frog, traffic, logs, and lillies

*/

#ifndef GAME_H
#define GAME_H

#include "Vehicle.h"
#include "Arena.h"
#include "Car.h"
#include <vector>
#include "Frog.h"
#include "Traffic.h"
#include <mutex>
#include "Lillies.h"
/***************** Helper functions **********************/

/*
Function: intersects
Purpose: returns true if the two passed rectangles intersect
@param: two rectangle shape objects
@return: true if the two rectangles intersect, false otherwise

*/
bool intersects(sf::RectangleShape rect1, sf::RectangleShape rect2);

//class definition for Game
class Game {

private:

	Frog *frog; //frog member variable

	Traffic traffic; //traffic variable
	sf::RenderWindow *window;
	sf::Text *livesText;
	
	bool frogDead;
	bool endOfGame; //bool that determines if the game is over
	int difficulty; //difficulty selected by the user
	bool win;
	int numFrogsOnLillies; //number of frogs on lillies
	//rectangles for the arena 
	sf::RectangleShape *road; //road and water rectangles drawn everytime the window is updated
	sf::RectangleShape *water;
	std::vector<Lilly> lillies; //lillies at the top of the screen

public: 
	Game(); //overloaded default constructors

	//getters / setters 
	Traffic getTraffic();
	Frog* Game::getFrog();
	bool did_game_end();
	void setEndOfGame(bool endOfGame);
	void frogDied();
	bool didFrogDie();
	bool getWin();
	sf::RenderWindow* getWindow();
	void Game::createWindow();
	void setDifficulty(int difficulty);
	int getDifficulty();
	std::vector<Lilly> *Game::getLillies();
	void setDifficultyOfTraffic();
	int getNumFrogsOnLillies();
	void Game::newFrog();

	//functions used to update the screen
	void Game::updateScreen();
	
	//functions for printing different menus
	void Game::printWelcomeMenu();
	void Game::printDifficultyMenu();
	int Game::printLoserScreen();
	int Game::printWinnerScreen();

	//increment number of frogs on lillies
	void incrementNumFrogsOnLillies();


	//collision detection
	bool detectLeftCollision();
	bool detectRightCollision();
	bool detectUpCollision();
	bool detectBottomCollision();
	bool Game::detectTrafficCollision();
	
	//used to detect if a frog is on/jumped on/jumped off a log
	int Game::jumpOnLog();
	bool Game::moveOnLog(bool right);
	int Game::jumpOffLog();
	
	//did the player jump on the lilly
	bool Game::jumpOnLilly();

	//mutex semaphores
	std::mutex traffic_mutex; //mutex semaphore for traffic array 

	std::mutex frog_mutex; //mutex semaphore for frog (might need)

	std::mutex endOfGame_mutex;

	std::mutex window_mutex;

	std::mutex dead_frog_mutex;

	sf::Clock clocks[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES]; //clock array used to make 
};


#endif