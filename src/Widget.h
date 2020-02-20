#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
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
	float width;
	float height;
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
	void setInput(std::string);
	void setPosition(sf::Vector2f);
	void setFontSize(int);
	void setLength(int);
	float getWidth() const;
	float getHeight() const;
	void draw(sf::RenderTarget&) const;
};


class ValueSetter
{
public:
	ValueSetter();
	virtual ~ValueSetter();

private:
	sf::Vector2f position;
	sf::RectangleShape backGroundRect;

	sf::CircleShape particleType;
	std::vector<std::unique_ptr<sf::CircleShape>> vecUpDown;

	sf::Font fontConsolas;
	sf::Text parameter;
	std::string sParameterName;
	sf::Text value;
	sf::String sValue;

	int minValue, maxValue, parameterValue, step;
	std::stringstream ss;

public:
	void setPosition(sf::Vector2f);
	void setParameterName(std::string);
	void setMinMaxValue(int, int);
	void setParameterValue(int);
	int getParameterValue() const;
	void setStep(float);

	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;

 };
