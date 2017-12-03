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
bool intersects(sf::RectangleShape rect1, sf::RectangleShape rect2);


class Game {

private:

	Frog *frog;

	Traffic traffic;
	sf::RenderWindow *window;
	sf::Text *livesText;
	bool frogDead;

	bool endOfGame;
	int difficulty;

	bool win;

	int numFrogsOnLillies;
	//rectangles for the arena
	sf::RectangleShape *road;
	sf::RectangleShape *water;
	
	std::vector<Lilly> lillies;

	float time;

public:
	Game();

	Traffic getTraffic();

	Frog* Game::getFrog();
	void Game::createWindow();

	bool did_game_end();
	void setEndOfGame(bool endOfGame);

	void frogDied();
	bool didFrogDie();
	void Game::newFrog();

	bool getWin();
	sf::RenderWindow* getWindow();

	void setDifficulty(int difficulty);
	int getDifficulty();
	std::vector<Lilly> *Game::getLillies();

	void Game::updateScreen();
	
	void Game::printWelcomeMenu();
	void Game::printDifficultyMenu();
	int getNumFrogsOnLillies();
	void incrementNumFrogsOnLillies();
	void setDifficultyOfTraffic();

	//collision detection
	bool detectLeftCollision();
	bool detectRightCollision();
	bool detectUpCollision();
	bool detectBottomCollision();

	bool Game::detectTrafficCollision();
	int Game::jumpOnLog();
	bool Game::moveOnLog(bool right);
	int Game::jumpOffLog();
	
	int Game::printLoserScreen();
	int Game::printWinnerScreen();

	bool Game::jumpOnLilly();

	//mutexes, currently public IMPLEMENT ENCAPSULATION LATER
	std::mutex traffic_mutex; //mutex semaphore for traffic array 

	std::mutex frog_mutex; //mutex semaphore for frog (might need)

	std::mutex endOfGame_mutex;

	std::mutex window_mutex;

	std::mutex dead_frog_mutex;

	sf::Clock clocks[NUMBER_OF_LANES][MAX_NUMBER_OF_VEHICLES];

	float getTime();
	void Game::setTime(float time);

};


#endif