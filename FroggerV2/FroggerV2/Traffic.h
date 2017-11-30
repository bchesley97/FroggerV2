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




};


#endif