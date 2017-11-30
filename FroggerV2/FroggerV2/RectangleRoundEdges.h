#pragma once
#ifndef RECTANGLEROUNDEDGES_H
#define RECTANGLEROUNDEDGES_H

#include <SFML/Graphics/shape.hpp>

class RectangleRoundEdges :public sf::Shape
{

private:

	sf::Vector2f size;
	float radius; //radius of edges
	uint32_t cornerPointCount;

public:

	RectangleRoundEdges(const sf::Vector2f &size, float rad, uint32_t pointCount);
	void setSize(const sf::Vector2f &size);
	void setEdgeRadius(float radius);
	float getEdgeRadius();
	sf::Vector2f getSize();

	std::size_t RectangleRoundEdges::getPointCount() const;
	sf::Vector2f RectangleRoundEdges::getPoint(std::size_t index) const;
};


#endif