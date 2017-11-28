#pragma once
#ifndef CAR_H
#define CAR_H
#include "Vehicle.h"

/************************************ Defines ********************************/



/* car is a subclass of vehicle. A car "IS" a vehicle */
class Car : public Vehicle {

	/*** all member variables are shared with Vehicle, but implementation will be different */
public:

	Car();
	Car(int centerX, int centerY, int speed, sf::Color color);


};

#endif