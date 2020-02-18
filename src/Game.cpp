#include "Game.h"

bool EngineAnalysis::operator<(const EngineAnalysis& rhs)
{
	return this->moveList < rhs.moveList;
}


void Game::print()
{
	if (vecPly.size() != 0)
	{
		const auto ply = vecPly.back();
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << " ply_nr=" << ply.ply_nr << "\ncurrent_moveList=\"" << ply.moveList << "\"" << std::endl;
		std::cout << " elapsed_time=" << ply.elapsed_time << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;

		for (auto& ea : ply.vecEA)
		{
			std::cout << "------------------------------------------------------------------------" << std::endl;
			std::cout << "      depth      " << ea.depth << std::endl;
			// std::cout << "      seldepth   " << ea.seldepth << std::endl;
			std::cout << "      multipv    " << ea.multipv << std::endl;
			std::cout << "      score_type " << ea.scoreType << std::endl;
			std::cout << "      score      " << ea.score << std::endl;
			// std::cout << "      nodes      " << ea.nodes << std::endl;
			std::cout << "      nps        " << ea.nps << std::endl;
			// std::cout << "      hashfull   " << ea.hashfull << std::endl;
			// std::cout << "      tbhits     " << ea.tbhits << std::endl;
			// std::cout << "      time       " << ea.time << std::endl;
			std::cout << "      moveList   " << ea.moveList << std::endl;
			std::cout << "------------------------------------------------------------------------" << std::endl;
		}
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
}
