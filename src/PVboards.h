#pragma once

#include <vector>

#include "ChessBoard.h"


class PVboards
{
public:
	PVboards PVboards();

private:
	size_t numberBoards;
	std::vector<ChessBoard> vecPVboards;

};
