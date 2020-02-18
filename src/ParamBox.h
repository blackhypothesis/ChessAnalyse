#pragma once

#include <iostream>
#include <vector>
#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>

#include "TextInput.h"

class ParamBox
{
public:
	ParamBox(sf::Vector2f, float);

private:
	sf::Vector2f position;
	std::vector<TextInput> vecParam;
	float width;

public:
	void update(std::vector<std::string>);
	void draw(sf::RenderWindow&) const;
};
