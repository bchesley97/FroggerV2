#ifndef GAME_H
#define GAME_H

#include "Vehicle.h"
#include "Arena.h"
#include "Car.h"
#include <vector>
#include "Frog.h"
#include "Traffic.h"
#include <mutex>
/***************** Helper functions **********************/
bool intersects(sf::RectangleShape rect1, sf::RectangleShape rect2);


class Game {

private:

	Frog *frog;

	Traffic traffic;
	sf::RenderWindow *window;

	bool endOfGame;
	int difficulty;

	bool win;
	//rectangles for the arena
	sf::RectangleShape *road;
	sf::RectangleShape *water;
	
	std::vector<sf::RectangleShape> lillies;


public:
	Game();

	Traffic getTraffic();

	Frog* Game::getFrog();

	bool did_game_end();
	void setEndOfGame(bool endOfGame);

	bool getWin();
	sf::RenderWindow* getWindow();

	std::vector<sf::RectangleShape> *getLillies();

	void Game::updateScreen();

	//collision detection
	bool detectLeftCollision();
	bool detectRightCollision();
	bool detectUpCollision();
	bool detectBottomCollision();

	bool Game::detectTrafficCollision();
	int Game::jumpOnLog();
	bool Game::moveOnLog(bool right);
	int Game::jumpOffLog();

	bool Game::jumpOnLilly();

	//mutexes, currently public IMPLEMENT ENCAPSULATION LATER
	std::mutex traffic_mutex; //mutex semaphore for traffic array 

	std::mutex frog_mutex; //mutex semaphore for frog (might need)

	std::mutex endOfGame_mutex;

	std::mutex window_mutex;

	sf::Clock clocks[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES];


};


#endif