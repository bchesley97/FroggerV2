#pragma once
#ifndef FROG_H
#define FROG_H

#include "Vehicle.h"


class Frog {

private:
	sf::RectangleShape *shape;
	const int jump_length = VEHICLE_LENGTH;
	const int horiz_jump = VEHICLE_LENGTH;
	int lane;
	int speed; //this variable is only if the frog has jumped on a log
	int logLane;
	int lives;

public:

	Frog();

	sf::RectangleShape* getShape();
	const int getJump();
	const int getHorizJump();
	int getLane();
	int getLogLane();
	void setLogLane(int logLane);

	void Frog::incrementLane();
	void Frog::decrementLane();

	void decrementLives();
	int getLives();
	void reset();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();


};


#endif