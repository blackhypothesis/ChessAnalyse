#include "TextInput.h"

TextInput::TextInput() :
		position(10.0f, 10.0f), maxTextLength(32), fontSize(15), input(""), textInputStr("")
{
	namespace fs = std::filesystem;
	auto cwp = fs::current_path();
	std::cout << "current path = " << cwp << std::endl;
	fontConsolas.loadFromFile("/home/marcel/workspace/ChessAnalyse/assets/Consolas.ttf");

	init();
}


TextInput::TextInput(sf::Vector2f position) :
		TextInput()
{
	this->position = position;
	init();
}


void TextInput::init()
{
	// approximated size of input field for font Consolas.ttf
	float backgroundWidth = fontSize * maxTextLength * 0.59f;
	float backgroundHeight = fontSize * 1.6f;
	sf::Vector2f textOffset = { 5.0f, 3.0f };
	backgroundRect.setSize( { backgroundWidth, backgroundHeight });
	backgroundRect.setFillColor(sf::Color( { 64, 64, 64 }));
	backgroundRect.setOutlineColor(sf::Color( { 128, 128, 128 }));
	backgroundRect.setOutlineThickness(1.0f);
	backgroundRect.setPosition(position);

	text.setFont(fontConsolas);
	text.setCharacterSize(fontSize);
	text.setFillColor(sf::Color::White);
	text.setPosition(position + textOffset);

	text.setString("<enter command>");
}


void TextInput::processEvent(sf::Event event)
{
	if (event.text.unicode < 128)
	{
		input = "";
		// return key: set input
		if (event.text.unicode == 0x0d)
		{
			input = textInputStr;
			textInputStr = "";
			text.setString(textInputStr);
		}
		// backspace key: remove last character
		else if (event.text.unicode == 0x08)
		{
			textInputStr = textInputStr.substr(0, textInputStr.size() - 1);
			text.setString(textInputStr);
		}
		else
		{
			if (textInputStr.size() < maxTextLength)
			{
				textInputStr += event.text.unicode;
				text.setString(textInputStr);
			}
		}
	}
}

std::string TextInput::getInput()
{
	// only return once the input string
	std::string tmp = input;
	input = "";
	return tmp;
}

void TextInput::setInput(std::string msg)
{
	text.setString(msg);
}

void TextInput::setPosition(sf::Vector2f position)
{
	this->position = position;
	init();
}

void TextInput::setFontSize(int fontSize)
{
	this->fontSize = fontSize;
	text.setCharacterSize(fontSize);
}

void TextInput::setLength(int length)
{
	this->maxTextLength = length;
	init();
}
void TextInput::draw(sf::RenderWindow &target) const
{
	target.draw(backgroundRect);
	target.draw(text);
}

