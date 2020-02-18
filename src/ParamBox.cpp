#include "ParamBox.h"

ParamBox::ParamBox(sf::Vector2f inPosition, float inWidth) :
		width(inWidth)
{
	position = inPosition;

	for (size_t x = 0; x < 3; x++)
	{
		for (size_t y = 0; y < 2; y++)
		{
			TextInput ti;
			ti.setPosition(position + sf::Vector2f(x * 20, y * width / 3));
			ti.setFontSize(14);
			ti.setLength(20);
			ti.setInput("");
			vecParam.push_back(ti);
		}
	}
}

void ParamBox::update(std::vector<std::string> vecStr)
{
	for (size_t i = 0; i < vecParam.size(); i++)
	{
		vecParam[i].setInput(vecStr[i]);
	}
}

void ParamBox::draw(sf::RenderWindow &target) const
{
	for (auto &param : vecParam)
	{
		param.draw(target);
	}
}
