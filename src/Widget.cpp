#include "Widget.h"

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
	width = fontSize * maxTextLength * 0.59f;
	height = fontSize * 1.6f;
	sf::Vector2f textOffset = { 5.0f, 3.0f };
	backgroundRect.setSize( { width, height });
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

float TextInput::getWidth() const
{
	return width;
}

float TextInput::getHeight() const
{
	return height;
}

void TextInput::draw(sf::RenderTarget &target) const
{
	target.draw(backgroundRect);
	target.draw(text);
}


ValueSetter::ValueSetter()
{
	backGroundRect.setSize({ 110.0f, 40.0f });
	backGroundRect.setFillColor(sf::Color{ 32, 32, 32 });
	backGroundRect.setOutlineColor(sf::Color{ 64, 64, 64 });
	backGroundRect.setOutlineThickness(2.0f);

	vecUpDown.push_back(std::make_unique<sf::CircleShape>(4.0f, 3));
	vecUpDown.push_back(std::make_unique<sf::CircleShape>(4.0f, 3));

	for (auto& s : vecUpDown)
	{
		s->setFillColor(sf::Color::Black);
		s->setOutlineColor(sf::Color{ 96, 96, 96 });
		s->setOutlineThickness(2.0f);
	}
	vecUpDown[1]->rotate(180.0f);

	fontConsolas.loadFromFile("/home/marcel/workspace/ChessAnalyse/assets/Consolas.ttf");

	parameter.setFont(fontConsolas);
	parameter.setCharacterSize(16);
	parameter.setFillColor(sf::Color::White);

	value.setFont(fontConsolas);
	value.setCharacterSize(16);
	value.setFillColor(sf::Color::White);

	minValue = -1.0f;
	maxValue = 1.0f;
	step = 0.1f;
	parameterValue = 0.0f;

}



ValueSetter::~ValueSetter()
{
}


void ValueSetter::setPosition(sf::Vector2f inPosition)
{
	position = inPosition + sf::Vector2f{2.0f, 2.0f};

	backGroundRect.setPosition(position);

	vecUpDown[0]->setPosition(position + sf::Vector2f{ 7.0f, 28.0f });
	vecUpDown[1]->setPosition(position + sf::Vector2f{ 30.0f, 32.0f });

	parameter.setPosition(position + sf::Vector2f{ 5.0f, 1.0f });
	parameter.setString(sParameterName);

	value.setPosition(position + sf::Vector2f{ 38.0f, 20.0f });
}


void ValueSetter::setParameterName(std::string inParameterName) { sParameterName = inParameterName; }


void ValueSetter::setMinMaxValue(int inMinValue, int inMaxValue)
{
	minValue = inMinValue;
	maxValue = inMaxValue;
}


void ValueSetter::setParameterValue(int inParameterValue)
{
	parameterValue = inParameterValue;
	if (parameterValue < minValue)
	{
		parameterValue = minValue;
		vecUpDown[0]->setFillColor(sf::Color::Black);
		vecUpDown[1]->setFillColor(sf::Color::Red);
	}
	else if (parameterValue > maxValue)
	{
		parameterValue = maxValue;
		vecUpDown[0]->setFillColor(sf::Color::Red);
		vecUpDown[1]->setFillColor(sf::Color::Black);
	}
	else
	{
		vecUpDown[0]->setFillColor(sf::Color::Black);
		vecUpDown[1]->setFillColor(sf::Color::Black);
	}

	ss.str(std::string());
	ss << std::fixed << std::setprecision(2) << parameterValue;
	value.setString(ss.str());
}


int ValueSetter::getParameterValue() const { return parameterValue; }

void ValueSetter::setStep(float inStep) { step = inStep; }


bool ValueSetter::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	bool isEdited = false;
	if (buttonPressed)
	{
		if (mousePos.x > position.x + 3.0f && mousePos.x < position.x + 18.0f
			&& mousePos.y > position.y + 24.0f && mousePos.y < position.y + 38.0f)
		{
			isEdited = true;
			setParameterValue(parameterValue + step);
		}
		else if (mousePos.x > position.x + 19.0f && mousePos.x < position.x + 34.0f
			&& mousePos.y > position.y + 24.0f && mousePos.y < position.y + 38.0f)
		{
			isEdited = true;
			setParameterValue(parameterValue - step);
		}
	}
	return isEdited;
}


void ValueSetter::draw(sf::RenderTarget& target) const
{
	target.draw(backGroundRect);

	for (auto& s : vecUpDown)
		target.draw(*s);

	target.draw(parameter);
	target.draw(value);
}
