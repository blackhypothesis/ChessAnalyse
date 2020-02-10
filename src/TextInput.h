#pragma once

#include <iostream>
#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>

class TextInput
{
public:
	TextInput();
	TextInput(sf::Vector2f);

private:
	sf::Vector2f position;
	sf::RectangleShape backgroundRect;
	sf::Text text;
	unsigned int maxTextLength;
	unsigned int fontSize;
	std::string input;
	std::string textInputStr;
	sf::Font fontConsolas;

	void init();

public:
	void processEvent(sf::Event);
	std::string getInput();
	void draw(sf::RenderWindow&) const;
};
