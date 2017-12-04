/*
File: Lillies.h
Purpose: Holds class definition for Lillies
*/


#ifndef LILLIES_H
#define LILLIES_H
#include <SFML/Graphics.hpp>

class Lilly {

private:

	sf::RectangleShape shape;
	bool frogOn;

public:
	//constructor for lilly
	Lilly::Lilly();
	//getters / setters 
	sf::RectangleShape *getShape();
	bool getFrogOn();
	void frogOnLilly();


};

#endif