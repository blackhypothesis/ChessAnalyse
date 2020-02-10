#pragma once

#include <sstream>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <regex>
#include <vector>
#include <string>

#include "ThreadSaveQueue.h"
#include "Game.h"

class Parser
{
public:
	Parser() = delete;
	Parser(ThreadSaveQueue&, ThreadSaveQueue&, ThreadSaveQueue&, Game&);

private:
	ThreadSaveQueue& stdOut;
	ThreadSaveQueue& userStdOut;
	ThreadSaveQueue& instruction;
	Game& game;

	EngineAnalysis ea;
	std::string command;
	unsigned int current_ply;
	std::string current_moveList;
	unsigned int current_depth;
	unsigned int current_pv;
	unsigned int current_pv_count;

	bool new_ply;
	bool new_depth;
	bool new_pv;

	enum GameState { NEW, AUTO, STOP } gameState;
	enum ViewState { NONE, SHORT } viewState;
	enum ChessComState { DISABLED, ENABLED } chessComState;

	std::string bestMove;
	std::string goDepthInstruction;

	const std::chrono::milliseconds wait_ms;

	unsigned long long int start_time;

	unsigned int hash_value;
	unsigned int threads_value;
	unsigned int multiPV_value;

	bool waitForReadyOK;

	void init();
	std::vector<std::string> getVecMove(std::string);


public:
	std::vector<std::string> parse(std::string);
	std::vector<std::string> parseUser(std::string);
	void printCurrentParams();
	void printShortInfo(Ply);
	void printInfo();
	void operator()();
};
