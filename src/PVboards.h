#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "ChessBoard.h"
#include "ThreadSaveQueue.h"
#include "Game.h"
#include "Widget.h"


class PVboards
{
public:
	PVboards(ThreadSaveQueue&);

private:
	ThreadSaveQueue& userStdOut;

	size_t numberBoards;
	float fieldSize;
	std::vector<ChessBoard> vecBoard;
	size_t numberInfo;
	std::vector<std::vector<TextInput>> vecText;
	ValueSetter valueThreads;

	void adjust();
	std::string numWithCommas(const std::string&);

public:
	void update(Game);
	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;

};
