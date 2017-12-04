/*
File:Vehcle.cpp
Purpose: Holds all implementation of the vehicle class

*/


#include "Vehicle.h"
#include <iostream>
/**************************************** Constructors ***********************************************/

/*
Default constructor for Vehicle

*/
Vehicle::Vehicle() {


	this->shape = nullptr;

	this->speed = 0;

}

/*
Constructor for Vehicle class which sets the position, size, speed, and color of the Vehicle

*/
Vehicle::Vehicle(int centerX, int centerY, int speed, int width, int length, sf::Color color) {

	this->shape = new sf::RectangleShape(sf::Vector2f(width , length ));
	this->speed = speed;
	this->shape->setPosition(centerX, centerY);
	this->shape->setFillColor(color);

}


/********************************** Getters/Setter methods *******************************************/


int Vehicle::getSpeed() {

	return this->speed;

}


int Vehicle::getLane() {

	return this->lane;

}

sf::RectangleShape* Vehicle::getShape() {
	return this->shape;
}

void Vehicle::setSpeed(int speed) {
	this->speed = speed;
}


void Vehicle::setLane(int lane) {

	this->lane = lane;

}

