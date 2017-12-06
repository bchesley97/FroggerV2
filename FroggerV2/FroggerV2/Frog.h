/*
File: Frog.h
Purpose: Holds class definition for the Frog
*/

#pragma once
#ifndef FROG_H
#define FROG_H

#include "Vehicle.h"


class Frog {

private:
	sf::RectangleShape *shape;
	const int jump_length = VEHICLE_LENGTH; //different jump lengths
	const int horiz_jump = VEHICLE_LENGTH;
	int lane; //lane the frog is in
	int speed; //this variable is only if the frog has jumped on a log
	int logLane; //log index used if the frog jumped onto a log
	int lives; //amount of lives 

public:

	//constructor
	Frog();

	//getters / setters methods
	sf::RectangleShape* getShape();
	const int getJump();
	const int getHorizJump();
	int getLane();
	int getLogLane();
	void setLogLane(int logLane);
	int getLives();
	
	void Frog::incrementLane(); //increment lane of frog
	void Frog::decrementLane(); //decrement lane of frog
	void decrementLives(); //frog died
	void setSpeed(int speed);

	void reset(); //reset cettain variables of the frog
	void moveLeft(); //move across lanes
	void moveRight();
	void moveUp();
	void moveDown();


};


#endif