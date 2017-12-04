/*
File: Traffic.cpp
Purpose: Holds all implementation for the traffic class
*/

#include "Traffic.h"
/*
Traffic default constructor. Creates a vector of vectors of both vehicles and logs. Sets the 
speed and position of the traffic

*/
Traffic::Traffic()
{
	//set up log traffic vector
	for (int i = 0; i<NUMBER_OF_LANES/2; i++)
	{
		logTraffic.push_back(std::vector<Log*>());
		int speed = (rand() % 2);
		int xPosition = 0;
		//determine whether to make speed negative (and thus start on the right side of the screen
		if (speed)
		{
			speed = -((rand() % MAX_VEHICLE_SPEED) + 5);

		}
		else
		{
			speed = (rand() % MAX_VEHICLE_SPEED) + 5;

		}

		for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
		{
			int spacing = 0;
			if (j != 0)
			{
				spacing = logTraffic[i].at(j-1)->getShape()->getSize().x + logTraffic[i].at(j-1)->getShape()->getPosition().x;
			}
			int differential = (rand() % FROG_SIZE) + FROG_SIZE;
			logTraffic[i].push_back(new Log(spacing + differential, (i + 1)*FROG_SIZE, speed));
			logTraffic[i][j]->setLane(i);
			logTraffic[i][j]->setSpeed(speed);
		}
	}

	//set up road traffic vector
	for (int i = 0 ; i<NUMBER_OF_LANES/2; i++)
	{
		roadTraffic.push_back(std::vector<Vehicle*>());
		int speed = (rand() % 2);
		int xPosition = 0;
		//determine whether to make speed negative (and thus start on the right side of the screen
		if (speed)
		{
			speed = -((rand() % MAX_VEHICLE_SPEED) + 5);

		}
		else
		{
			speed = (rand() % MAX_VEHICLE_SPEED) + 5;
		}

		for (int j = 0; j < MAX_NUMBER_OF_VEHICLES; j++)
		{
			int differential = (rand() % FROG_SIZE) + FROG_SIZE;
			roadTraffic[i].push_back(new Car(j*2*FROG_SIZE + differential + j*(FROG_SIZE + differential), (i + 1+NUMBER_OF_LANES/2)*FROG_SIZE, speed, sf::Color::Black));
			roadTraffic[i][j]->setLane(i+NUMBER_OF_LANES/2);
			roadTraffic[i][j]->setSpeed(speed);
		}
	}
}
//getter methods
std::vector<std::vector<Vehicle*> > *Traffic:: getRoadTraffic()
{
	return &roadTraffic;
}
std::vector<std::vector<Log*> > *Traffic::getLogTraffic()
{
	return &logTraffic;
}