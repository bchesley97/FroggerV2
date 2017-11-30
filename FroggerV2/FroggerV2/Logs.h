#pragma once

#ifndef LOGS_H
#define LOGS_H
#include "RectangleRoundEdges.h"

class Log {

private:

	int speed;
	RectangleRoundEdges *shape;
	int lane;

public:

	Log(int xPos, int yPos, int speed);

	void setSpeed(int speed);
	void setLane(int lane);
	int getSpeed();
	RectangleRoundEdges* getShape();
	int getLane();
};

#endif