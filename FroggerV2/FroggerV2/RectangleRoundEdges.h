#pragma once
#ifndef RECTANGLEROUNDEDGES_H
#define RECTANGLEROUNDEDGES_H

#include <SFML/Graphics/shape.hpp>

class RectangleRoundEdges :public sf::Shape
{

private:

	sf::Vector2f size;
	float radius; //radius of edges

public:

	RectangleRoundEdges(const sf::Vector2f &size, float rad);
	void setSize(const sf::Vector2f &size);
	void setEdgeRadius(float radius);
	float getEdgeRadius();
	sf::Vector2f getSize();

	std::size_t RectangleRoundEdges::getPointCount() const
	{
		return 0; //do not plan on using this 
	}

	sf::Vector2f RectangleRoundEdges::getPoint(std::size_t index) const
	{
		return sf::Vector2f(0, 0); //not correct implementation but will 
	}

};


#endif