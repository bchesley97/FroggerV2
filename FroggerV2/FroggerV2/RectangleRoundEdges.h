/*
File: RectangleRoundEdges.h
Purpose: Holds class defintion for RectangleRoiundEdges. This is a rectangle with a semicircle at both edges.
		The logs are represented as a rectangleRoundEdges

*/

#pragma once
#ifndef RECTANGLEROUNDEDGES_H
#define RECTANGLEROUNDEDGES_H

#include <SFML/Graphics/shape.hpp>

class RectangleRoundEdges :public sf::Shape
{

private:

	sf::Vector2f size; //member variables 
	float radius; //radius of edges
	uint32_t cornerPointCount;

public:
	//constructor
	RectangleRoundEdges(const sf::Vector2f &size, float rad, uint32_t pointCount);
	//virtual methods needed to implement to instantiate a class
	void setSize(const sf::Vector2f &size);
	void setEdgeRadius(float radius);
	float getEdgeRadius();
	sf::Vector2f getSize();

	std::size_t RectangleRoundEdges::getPointCount() const;
	sf::Vector2f RectangleRoundEdges::getPoint(std::size_t index) const;
};


#endif