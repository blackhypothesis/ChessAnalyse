#include "Arrow.h"

Arrow::Arrow() :
		length(150), theta(0), bodyThickness(10), topThickness(20), topLenght(20)
{
	init();
}

void Arrow::init()
{
	sf::Vector2f topOrigin = sf::Vector2f(-length + topLenght, -0);

	body.setPointCount(4);
	body.setPoint(0, sf::Vector2f(0, -bodyThickness / 2));
	body.setPoint(1, sf::Vector2f(0, bodyThickness / 2));
	body.setPoint(2, sf::Vector2f(length - topLenght, bodyThickness / 2));
	body.setPoint(3, sf::Vector2f(length - topLenght, -bodyThickness / 2));
	body.setOrigin(sf::Vector2f(0, 0));
	body.setPosition(position);
	body.setRotation(theta * 180 / 3.141592653);

	top.setPointCount(3);
	top.setPoint(0, sf::Vector2f(topLenght, 0));
	top.setPoint(1, sf::Vector2f(0, -topThickness / 2));
	top.setPoint(2, sf::Vector2f(0, topThickness / 2));
	top.setOrigin(topOrigin);
	top.setPosition(position);
	top.setRotation(theta * 180 / 3.141592653);

	center.setRadius(8);
	center.setOrigin(8, 8);
	center.setPosition(position);
	center.setFillColor(sf::Color::Red);

}

void Arrow::setBeginEnd(sf::Vector2f begin, sf::Vector2f end)
{
	position = begin;

	float deltaX = end.x - begin.x;
	float deltaY = end.y - begin.y;
	theta = atan2f(deltaY, deltaX);
	length = sqrt(deltaX * deltaX + deltaY * deltaY);

	init();

}

void Arrow::rotate(float rotate)
{
	body.rotate(rotate);
	top.rotate(rotate);
}

void Arrow::setColor(sf::Color color)
{
	body.setFillColor(color);
	top.setFillColor(color);
}

void Arrow::draw(sf::RenderTarget &target) const
{
	// target.draw(center);
	target.draw(body);
	target.draw(top);
}
