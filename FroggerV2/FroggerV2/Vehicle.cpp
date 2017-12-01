
#include "Vehicle.h"
#include <iostream>
/**************************************** Constructors ***********************************************/

Vehicle::Vehicle() {


	this->shape = nullptr;

	this->speed = 0;

}


Vehicle::Vehicle(int centerX, int centerY, int speed, int width, int length, sf::Color color) {


	this->shape = new sf::RectangleShape(sf::Vector2f(width , length ));
	this->speed = speed;
	//this->width_d2 = size_d2;

	//this->shape->setOrigin(centerX, centerY);
	this->shape->setPosition(centerX, centerY);

	this->shape->setFillColor(color);
	//this->width_d2 = size_d2;
	//std::cout << "In  vehicle constructor\n";

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

