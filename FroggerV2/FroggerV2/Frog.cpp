/*
File: Frog.cpp
Purpose: Holds implementation of Frog class

*/


#include "Frog.h"
/*
Function: Frog (constructor)
Purpose: sets the frogs member variables. Sets its color to green, 
		 speed to zero, number of lives to 3, and the shapes position and size

*/
Frog::Frog()
{
	shape = new sf::RectangleShape(sf::Vector2f(FROG_SIZE, FROG_SIZE));
	shape->setFillColor(sf::Color::Green); //frogs are usually green
	shape->setPosition(WINDOW_MAX_X / 2, NUMBER_OF_LANES*(FROG_SIZE) + FROG_SIZE);
	lane = NUMBER_OF_LANES;
	speed = 0; //no speed at first
	logLane = -1;
	lives = 3; //three lives 
}

//getter methods
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
int Frog::getLives()
{
	return lives;
}
//setter method
void Frog::setLogLane(int logLane)
{
	this->logLane = logLane;
}

//increments the lane of the frog
void Frog::incrementLane()
{
	++lane;
}
//decrements the lane of the frog
void Frog::decrementLane()
{
	--lane;
}
//movement methods for the frog
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
//frog has died, decrement the number of lives it has
void Frog::decrementLives()
{
	--lives;
}
/*
Function: Reset
Purpose: reset certain variables of the frog, specifially its position, lane, and speed.
@param void
@return void
*/
void Frog::reset()
{
	shape->setPosition(WINDOW_MAX_X / 2, NUMBER_OF_LANES*(FROG_SIZE)+FROG_SIZE); //reset frog
	lane = NUMBER_OF_LANES;
	speed = 0; //no speed at first
	logLane = -1;
}