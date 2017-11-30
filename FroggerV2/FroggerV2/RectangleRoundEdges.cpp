#include "RectangleRoundEdges.h"
#include <cmath>

RectangleRoundEdges::RectangleRoundEdges(const sf::Vector2f &size, float rad, uint32_t pointCount)
{
	this->size = size;
	this->radius = rad;
	cornerPointCount = pointCount;
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

std::size_t RectangleRoundEdges::getPointCount() const
{
	return cornerPointCount * 4;
}
sf::Vector2f RectangleRoundEdges::getPoint(std::size_t index) const
{
	if (index >= cornerPointCount * 4)
	{
			return sf::Vector2f(0, 0);
	}

	float angle = 90.0f / (cornerPointCount - 1);
	sf::Vector2f center;
	uint32_t index_center = index / cornerPointCount;
	uint32_t set = 0;

	if (index_center == 0)
	{
		center.y = radius;
		center.x = size.x - radius;
	}
	else if (index_center == 1)
	{
		center.y = radius;
		center.x = radius;
	}
	else if (index_center == 2)
	{
		center.x = radius;
		center.y = size.y - radius;
	}
	else if (index_center == 3)
	{
		center.x = size.x - radius;
		center.y = size.y - radius;

	}
	double PI = 3.14159265f;
	return sf::Vector2f(radius*cos(angle*(index - index_center)*PI / 180) + center.x,
		-radius*sin(angle*(index - index_center)*PI / 180) + center.y);


}