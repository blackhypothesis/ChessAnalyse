#include "Game.h"

bool EngineAnalysis::operator<(const EngineAnalysis& rhs)
{
	return this->moveList < rhs.moveList;
}
