#include "Lillies.h"
#include "Arena.h"
Lilly::Lilly()
{
	shape = (*new sf::RectangleShape(sf::Vector2f(LILY_PAD_WIDTH, LILY_PAD_LENGTH)));
	shape.setFillColor(sf::Color(102, 238, 148, 255)); //some kind of light green
}
sf::RectangleShape* Lilly::getShape()
{
	return &shape;
}
bool Lilly::getFrogOn()
{
	return frogOn;
}
void Lilly::frogOnLilly()
{
	frogOn = true;
	shape.setFillColor(sf::Color::Green); //make lilly green since theres a frog on it
}
