#pragma once

#include <string>
#include <vector>


struct EngineAnalysis
{
	// this are variables from the info output
	unsigned int depth;
	unsigned int seldepth;
	unsigned int multipv;
	std::string score_type;
	int score;
	unsigned int nodes;
	unsigned int nps;
	unsigned int hashfull;
	unsigned int tbhits;
	unsigned int time;
	std::string moveList;
	std::vector<std::string> pv;

	bool operator<(const EngineAnalysis&);
};


struct Ply
{
	unsigned int ply_nr;
	std::string moveList;
	unsigned long long int elapsed_time;

	std::vector<EngineAnalysis> vecEA;
};


struct Game
{
	std::vector<Ply> vecPly;
};

