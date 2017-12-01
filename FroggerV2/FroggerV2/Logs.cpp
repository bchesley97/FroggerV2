#pragma once
#include "Logs.h"
#include "Arena.h"
Log::Log(int xPos, int yPos, int speed)
{
	int randomWidth = MIN_LOG_WIDTH + (rand() % 75);
	shape = new RectangleRoundEdges(sf::Vector2f(randomWidth, LOG_LENGTH), 10, 30);
	shape->setPosition(xPos, yPos);
	
	//implement log picture
	shape->setFillColor(sf::Color(101, 67, 33,255)); //should be dark brown
	this->speed = speed;


}

int Log::getSpeed()
{
	return speed;
}
RectangleRoundEdges* Log:: getShape()
{
	return shape;
}

int Log::getLane()
{
	return lane;
}

void Log::setLane(int lane)
{
	this->lane = lane;
}

void Log::setSpeed(int speed)
{
	this->speed = speed;
}