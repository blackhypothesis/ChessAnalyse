#pragma once

#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>


class Arrow
{
public:
	Arrow();

private:
	sf::CircleShape center;
	sf::ConvexShape body;
	sf::ConvexShape top;
	sf::Vector2f position;
	float length;
	float theta;
	float bodyThickness;
	float topThickness;
	float topLenght;

	void init();

public:
	void setBeginEnd(sf::Vector2f, sf::Vector2f);
	void rotate(float);
	void setTransparency(int);
	void draw(sf::RenderTarget&) const;

};

