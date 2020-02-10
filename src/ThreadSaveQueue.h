#pragma once

#include <mutex>
#include <queue>
#include <vector>
#include <string>

#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>

class ThreadSaveQueue
{
public:
	ThreadSaveQueue(std::string);

private:
	const std::string queueName;
	std::mutex mtx;
	std::queue<std::string> msgQueue;
	std::ofstream logFile;

public:
	void push(std::string);
	void push(std::vector<std::string>);
	std::string pop();
	unsigned int size() const;
};
