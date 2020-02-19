#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "ChessBoard.h"
#include "Widget.h"


class PVboards
{
public:
	PVboards();

private:
	size_t numberBoards;
	float fieldSize;
	std::vector<ChessBoard> vecBoard;
	size_t numberInfo;
	std::vector<std::vector<TextInput>> vecText;

	void adjust();
	std::string numWithCommas(const std::string&);

public:
	void update(Game);
	void draw(sf::RenderWindow&) const;

};
