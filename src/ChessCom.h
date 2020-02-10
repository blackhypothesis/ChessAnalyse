#pragma once

#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>

#include <iostream>

#include "ThreadSaveQueue.h"

class ChessCom
{
public:
	ChessCom() = delete;
	ChessCom(ThreadSaveQueue&);
	void operator()();

private:
	ThreadSaveQueue& userStdOut;
	std::string filename;
	const std::chrono::milliseconds wait_ms;
	std::map<std::string, std::string> moveKey;

public:
	std::string getMoveList(const std::string&);
	void printMap() const;
};


