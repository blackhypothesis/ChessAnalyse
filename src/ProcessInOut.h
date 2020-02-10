#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <boost/process.hpp>
#include "ThreadSaveQueue.h"
#include "Game.h"

namespace bp = boost::process;
namespace bf = boost::filesystem;


class ChildProcInOut
{
public:
	ChildProcInOut() = delete;
	ChildProcInOut(std::string, ThreadSaveQueue&, ThreadSaveQueue&, ThreadSaveQueue&);

private:
	std::string childPath;
	bp::child childProcess;
	ThreadSaveQueue& stdIn;
	ThreadSaveQueue& stdOut;
	ThreadSaveQueue& instruction;
	bp::opstream os;
	bp::ipstream is;
	std::thread threadStdIn;
	std::thread threadStdOut;

	const std::string out_file;
	const std::chrono::milliseconds wait_ms;

public:
	void execChild();
	void operator()(unsigned int);
};
