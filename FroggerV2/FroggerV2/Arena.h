#pragma once
#ifndef ARENA_H
#define ARENA_H


/*********************************** Defines ****************************************/


/*
* Boundaries for the window
*/
#define WINDOW_MAX_X 800
#define WINDOW_MIN_X 0

#define WINDOW_MAX_Y 800
#define WINDOW_MIN_Y 0

#define BACK_COLOR (sf::Color::White)

/*
defines for frog (will probably be changed)
*/
#define FROG_SIZE (20)
#define FROG_SIZE_D2 (FROG_SIZE >> 1)
#define FROG_JUMP (VEHICLE_LENGTH)


/*
defines for traffic
*/

#define VEHICLE_LENGTH FROG_SIZE //so that vehicle length and frog size are equal
#define CAR_WIDTH (FROG_SIZE+10)
#define CAR_WIDTH_D2 (CAR_WIDTH>>1)

#define BUS_WIDTH 10
#define BUS_WIDTH_D2 (BUS_WIDTH>>1)

#define MAX_NUMBER_OF_VEHICLES (WINDOW_MAX_X/(4*FROG_SIZE)) //max number of vehicles on the screen at all times

#define MIN_SPACE_BETWEEN_VEHICLES (FROG_SIZE + 6) //minimum space between vehicles

#define NUMBER_OF_LANES ((int)(WINDOW_MAX_Y/VEHICLE_LENGTH) - 2)

#define SPACE_BETWEEN_LANES (FROG_SIZE)

#define MAX_VEHICLE_SPEED 5 // max speed of vehicles is 30 pixel "jumps" from left to right 



/************************* Data Structures *************************/




#endif
