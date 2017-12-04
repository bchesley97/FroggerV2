/*
File: Car.cpp 
Purpose: implementation fo the car class, just holds the constructors since a car is a vehicle
*/

#include "Car.h"
#include "Vehicle.h"
#include <iostream>

/**************************** Constructors ********************************/

Car::Car()
	:Vehicle() {

	//Vehicle();
	std::cout << "in car default constructor\n";

}

Car::Car(int centerX, int centerY, int speed, sf::Color color)
	: Vehicle::Vehicle(centerX, centerY, speed, FROG_SIZE, FROG_SIZE, color) {

//just calls its super class' constructo
}