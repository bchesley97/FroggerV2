#pragma once
#ifndef FROG_H
#define FROG_H

#include "Vehicle.h"


class Frog {

private:
	sf::RectangleShape *shape;
	const int jump_length = VEHICLE_LENGTH;
	int lane;

public:

	Frog();

	sf::RectangleShape* getShape();
	const int getJump();
	int getLane();
	void Frog::incrementLane();
	void Frog::decrementLane();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();


};


#endif