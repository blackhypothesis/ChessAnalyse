#include "ThreadSaveQueue.h"

ThreadSaveQueue::ThreadSaveQueue(std::string inQueueName) :
		queueName(inQueueName)
{
	logFile = std::ofstream("chesscomputer.log");
}

void ThreadSaveQueue::push(std::string msg)
{
	std::lock_guard<std::mutex> lock(mtx);
	msgQueue.push(msg);
}


void ThreadSaveQueue::push(std::vector<std::string> vecMsg)
{
	std::lock_guard<std::mutex> lock(mtx);

	for (auto& msg : vecMsg)
	{
		msgQueue.push(msg);
	}
}


std::string ThreadSaveQueue::pop()
{
	std::lock_guard<std::mutex> lock(mtx);
	std::string firstMsg = "NULL";

	if (msgQueue.size() > 0)
	{
		firstMsg = msgQueue.front();
		msgQueue.pop();

		std::ofstream logFile("chesscomputer.log", std::fstream::app);


		if (logFile.is_open())
		{
			// unsigned long long int ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

			logFile << queueName << "> " << firstMsg << "\n";
			logFile.flush();
		}
		else
		{
			std::cout << "cannot write to LOGFILE!!!" << std::endl;
		}
		logFile.close();
	}

	return firstMsg;
}

unsigned int ThreadSaveQueue::size() const
{
	return msgQueue.size();
}

