#include "ChessCom.h"

ChessCom::ChessCom(ThreadSaveQueue& inUserStdOut) :
		userStdOut(inUserStdOut),
		wait_ms(5)
{
	filename = "/home/marcel/proxy/chess.log";

	moveKey =
	{	{ "a", "a1" },	{ "i", "a2" },	{ "q", "a3" },	{ "y", "a4" },	{ "G", "a5" },	{ "O", "a6" },	{ "W", "a7" },	{ "4", "a8" },
		{ "b", "b1" },	{ "j", "b2" },	{ "r", "b3" },	{ "z", "b4" },  { "H", "b5" },	{ "P", "b6" },	{ "X", "b7" },	{ "5", "b8" },
		{ "c", "c1" },	{ "k", "c2" },	{ "s", "c3" },	{ "A", "c4" },	{ "I", "c5" },	{ "Q", "c6" },	{ "Y", "c7" },	{ "6", "c8" },
		{ "d", "d1" },	{ "l", "d2" },	{ "t", "d3" },	{ "B", "d4" },	{ "J", "d5" },	{ "R", "d6" },	{ "Z", "d7" },	{ "7", "d8" },
		{ "e", "e1" },	{ "m", "e2" },	{ "u", "e3" },	{ "C", "e4" },	{ "K", "e5" },	{ "S", "e6" },	{ "0", "e7" },	{ "8", "e8" },
		{ "f", "f1" },	{ "n", "f2" },	{ "v", "f3" },	{ "D", "f4" },	{ "L", "f5" },	{ "T", "f6" },	{ "1", "f7" },	{ "9", "f8" },
		{ "g", "g1" },	{ "o", "g2" },	{ "w", "g3" },	{ "E", "g4" },	{ "M", "g5" },	{ "U", "g6" },	{ "2", "g7" },	{ "!", "g8" },
		{ "h", "h1" },	{ "p", "h2" },	{ "x", "h3" },	{ "F", "h4" },	{ "N", "h5" },	{ "V", "h6" },	{ "3", "h7" },	{ "?", "h8" }	};
}

void ChessCom::operator()()
{

	std::string subject;
	std::ifstream ifs(filename);

	const std::regex rexMoveList(R"rex("moves":"([^"]+)",)rex");

	//const std::regex rexMoves(R"rex((moves))rex");

	const std::regex rexMoves(R"rex("moves":"([^"]+)",)rex");

	std::smatch match;

	std::cout << "starting chess.com thread" << std::endl;


	if (ifs.is_open())
	{
		while (true)
		{
			while (std::getline(ifs, subject))
			{

				try
				{
					if (std::regex_search(subject, match, rexMoveList))
					{
						// get obfuscted movelist from chess.com, translate it, and send it to usetStdOut queue
						const std::string chessComMoveList = match[1];
						const std::string moveList = getMoveList(chessComMoveList);
						const std::string chessComCommand = "chesscom movelist " + std::string("\"") + moveList + "\"";
						const std::string chessComInfo = "chesscom info movelist " + chessComMoveList;
						userStdOut.push(chessComInfo);
						userStdOut.push(chessComCommand);
					}
				}
				catch (std::regex_error& e)
				{
					// Syntax error in the regular expression
					std::cout << "regex not valid" << std::endl;
					std::cout << e.what() << std::endl;
				}
			}

			if (!ifs.eof())
				break;

			ifs.clear();

			// wait for some milliseconds in order to not hook on the CPU
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
		}
	}

}


std::string ChessCom::getMoveList(const std::string& line)
{
	std::string moveList;
	unsigned int i = 1;
	for (const char& c : line)
	{
		std::string s(1, c);
		std::map<std::string, std::string>::iterator p = moveKey.find(s);
		moveList = moveList + p->second;

		i++;
		if (i % 2)
			moveList = moveList + " ";
	}

	return moveList;
}


void ChessCom::printMap() const
{
	for (auto& p : moveKey)
	{
		std::cout << p.first << " " << p.second << std::endl;
	}
}

