#ifndef LILLIES_H
#define LILLIES_H
#include <SFML/Graphics.hpp>

class Lilly {

private:

	sf::RectangleShape shape;
	bool frogOn;

public:

	Lilly::Lilly();
	sf::RectangleShape *getShape();
	bool getFrogOn();
	void frogOnLilly();


};

#endif