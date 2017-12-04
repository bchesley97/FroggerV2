/*
File: Logs.h
Purpose: holds class definition for Log class. Each Log moves at a certain speed, in a certain lane, and is represented with a rectangle with round edges object
*/

#pragma once

#ifndef LOGS_H
#define LOGS_H
#include "RectangleRoundEdges.h"

class Log {

private:

	int speed; //member variables
	RectangleRoundEdges *shape;
	int lane;

public:
	//constructor
	Log(int xPos, int yPos, int speed);

	//getters / setters
	void setSpeed(int speed);
	void setLane(int lane);
	int getSpeed();
	RectangleRoundEdges* getShape();
	int getLane();
};

#endif