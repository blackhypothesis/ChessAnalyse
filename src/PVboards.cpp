#include "PVboards.h"

PVboards::PVboards() :
		numberBoards(4)
{
	for (size_t i = 0; i < numberBoards; i++)
	{
		vecPVboards.push_back(ChessBoard());
	}
}
