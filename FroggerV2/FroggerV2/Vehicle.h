#pragma once
/*
File: Vehicle.h
Purpose: Contains class definition for Vehicle, which will be the super class for both Car and Bus


*/


#ifndef VEHICLE_H
#define VEHICLE_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Arena.h"

/*

Each Vehicle is a rectangle

*/
class Vehicle {

private:

	int speed; //THIS SPEED IS IN PIXELS/SECOND
	//int width_d2;
	int lane;
	sf::RectangleShape *shape;

public:

	/* default constructor for vehicle*/
	Vehicle();


	/* constructor for vehicle */
	Vehicle::Vehicle(int centerX, int centerY, int speed, int width, int length, sf::Color color);
		/*
	int getCenterX();
	int getCenterY();
	*/
	int getSpeed();
	int getLane();
	sf::RectangleShape* getShape();

	/*
	void setCenterX(int centerX);
	void setCenterY(int centerY);

	*/
	void setSpeed(int speed);
	void setLane(int lane);

	/**** implementation methods ****/

};

#endif