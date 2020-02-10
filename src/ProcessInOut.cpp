#include "ProcessInOut.h"


ChildProcInOut::ChildProcInOut(std::string inChildPath,
		ThreadSaveQueue& inStdIn,
		ThreadSaveQueue& inStdOut,
		ThreadSaveQueue& inInstr) :
		childPath(inChildPath),
		stdIn(inStdIn),
		stdOut(inStdOut),
		instruction(inInstr),
		out_file("/home/marcel/tmp/out.log"),
		wait_ms(5)

{
}

void ChildProcInOut::execChild()
{
	childProcess = bp::child(childPath, bp::std_in < os, bp::std_out > is);

	threadStdIn = std::thread([this]
	{
		(*this)(0);
	});
	threadStdOut = std::thread([this]
	{
		(*this)(1);
	});
}

void ChildProcInOut::operator()(unsigned int fd)
{
	// stdin
	if (fd == 0)
	{
		while (true)
		{
			std::string command = instruction.pop();
			if (command != "NULL")
			{
				os << command << std::endl;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
		}

	}
	// stdout
	else if (fd == 1)
	{
		std::string line;

		while (std::getline(is, line))
		{
			stdOut.push(line);
		}
	}
}
