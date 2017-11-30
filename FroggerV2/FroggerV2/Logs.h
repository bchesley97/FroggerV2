#pragma once

#ifndef LOGS_H
#define LOGS_H
#include "RectangleRoundEdges.h"

class Log {

private:

	int speed;
	RectangleRoundEdges *shape;


public:

	Log(int xPos, int yPos, int speed);

	int getSpeed();
	RectangleRoundEdges* getShape();

};

#endif