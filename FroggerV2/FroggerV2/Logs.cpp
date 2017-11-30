#pragma once
#include "Logs.h"
#include "Arena.h"
Log::Log(int xPos, int yPos, int speed)
{
	int randomWidth = MIN_LOG_WIDTH + (rand() % 10);
	shape = new RectangleRoundEdges(sf::Vector2f(randomWidth, LOG_LENGTH), 3);
	shape->setPosition(xPos, yPos);
	
	//implement log picture
	shape->setFillColor(sf::Color(101, 67, 33)); //should be dark brown
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