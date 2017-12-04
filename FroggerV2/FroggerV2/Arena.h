/*
File: Arena.h
Purpose: This file holds all the defines used for the frogger game

*/

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
defines for frog 
*/
#define FROG_SIZE (50)
#define FROG_SIZE_D2 (FROG_SIZE >> 1)
#define FROG_JUMP (VEHICLE_LENGTH)


/*
defines for traffic
*/

#define VEHICLE_LENGTH FROG_SIZE //so that vehicle length and frog size are equal
#define CAR_WIDTH (FROG_SIZE)
#define CAR_WIDTH_D2 (CAR_WIDTH>>1)

#define BUS_WIDTH 10
#define BUS_WIDTH_D2 (BUS_WIDTH>>1)

#define MAX_NUMBER_OF_VEHICLES (WINDOW_MAX_X/(2*FROG_SIZE)) //max number of vehicles on the screen at all times

#define MIN_SPACE_BETWEEN_VEHICLES (FROG_SIZE + 6) //minimum space between vehicles

#define SPACE_BETWEEN_LANES (4)

#define NUMBER_OF_LANES ((int)(WINDOW_MAX_Y/VEHICLE_LENGTH) - 2) //NUMBER OF LANES OF TRAFFIC, 2 free lanes for frog starting and ending position

#define MAX_VEHICLE_SPEED 10 // max speed of vehicles is 5 pixel "jumps" 


/****************** Defines for lilies ******************/

#define NUMBER_OF_LILLIES 2
//(WINDOW_MAX_X/(3*FROG_SIZE))
#define LILY_PAD_LENGTH (FROG_SIZE)
#define LILY_PAD_WIDTH (FROG_SIZE) //ten is "wiggle room" on each side to land on

/********************* Defines for logs **********************/

#define LOG_LENGTH (FROG_SIZE)
#define MIN_LOG_WIDTH (FROG_SIZE*3)


/************* Character / text defines ****************/
#define CHAR_SIZE (20)
#define LIVES_TEXT_PLACEMENT_Y (WINDOW_MAX_Y - CHAR_SIZE-5)



#endif
