/*
File: Traffic.h
Purpose: Holds class definition for the Traffic Class. The traffic is composed of a vector of vectors of both Vehicles and Logs.
*/

#pragma once
#ifndef TRAFFIC_H
#define TRAFFIC_H

#include "Vehicle.h"
#include "Arena.h"
#include "Car.h"
#include <vector>
#include "Frog.h"
#include "Logs.h"

class Traffic {

private:

	//vector of vectors for both the road and log traffic
	std::vector<std::vector<Vehicle*> > roadTraffic; 
	std::vector<std::vector<Log*> > logTraffic;

public:

	//constructor
	Traffic();

	//getters, these variables are set in the constructor
	std::vector<std::vector<Vehicle*> > *getRoadTraffic();
	std::vector<std::vector<Log*> > *getLogTraffic();
};


#endif