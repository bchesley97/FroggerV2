#include "Frog.h"



Frog::Frog()
{
	shape = new sf::RectangleShape(sf::Vector2f(FROG_SIZE, FROG_SIZE));
	shape->setFillColor(sf::Color::Green); //frogs are usually green
	shape->setPosition(WINDOW_MAX_X / 2, WINDOW_MAX_Y - FROG_SIZE);
	lane = NUMBER_OF_LANES;
	speed = 0; //no speed at first
	logLane = -1;
}

sf::RectangleShape* Frog::getShape()
{
	return shape;
}

const int Frog::getJump()
{
	return jump_length;
}
const int Frog::getHorizJump()
{
	return horiz_jump;
}
int Frog::getLane()
{
	return lane;
}

int Frog::getLogLane()
{
	return speed;
}
void Frog::setLogLane(int logLane)
{
	this->logLane = logLane;
}

void Frog::incrementLane()
{
	++lane;
}
void Frog::decrementLane()
{
	--lane;
}

void Frog::moveLeft()
{
	shape->move(-horiz_jump, 0);

}
void Frog::moveRight()
{
	shape->move(horiz_jump, 0);

}
void Frog::moveUp()
{
	shape->move(0, -VEHICLE_LENGTH); //since top left corner is (0,0), need to add negative Y displacement

}
void Frog::moveDown()
{
	shape->move(0, VEHICLE_LENGTH);
}