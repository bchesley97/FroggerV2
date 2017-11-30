#include "RectangleRoundEdges.h"


RectangleRoundEdges::RectangleRoundEdges(const sf::Vector2f &size, float rad)
{
	this->size = size;
	this->radius = rad;
	update();
}



void RectangleRoundEdges::setSize(const sf::Vector2f &size)
{
	this->size = size;
	update();

}
void RectangleRoundEdges::setEdgeRadius(float radius)
{
	this->radius = radius;
	update();
}
float RectangleRoundEdges::getEdgeRadius()
{
	return radius;

}
sf::Vector2f RectangleRoundEdges::getSize()
{
	return size;
}