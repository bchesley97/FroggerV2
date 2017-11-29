#ifndef GAME_H
#define GAME_H

#include "Vehicle.h"
#include "Arena.h"
#include "Car.h"
#include <vector>
#include "Frog.h"

class Game {

private:

	Frog *frog;

	std::vector<std::vector<Vehicle*> > traffic;
	sf::RenderWindow *window;

	bool endOfGame;
	int difficulty;



public:
	Game();

	std::vector <std::vector<Vehicle*> > Game::getTraffic();

	Frog* Game::getFrog();

	bool did_game_end();
	void setEndOfGame(bool endOfGame);

	sf::RenderWindow* getWindow();
	//collision detection
	bool detectLeftCollision();
	bool detectRightCollision();
	bool detectUpCollision();
	bool detectBottomCollision();


};


#endif