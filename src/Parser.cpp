#include "Parser.h"

Parser::Parser(ThreadSaveQueue& inStdOut, ThreadSaveQueue& inUserStdOut, ThreadSaveQueue& inInstr, Game& inGame) :
		stdOut(inStdOut),
		userStdOut(inUserStdOut),
		instruction(inInstr),
		game(inGame),
		wait_ms(5)

{
	init();
}

void Parser::init()
{
	game.vecPly.clear();
	current_ply = 0;
	current_moveList = "";
	current_depth = 0;
	current_pv = 0;
	current_pv_count = 0;
	new_ply = true;
	new_depth = true;
	new_pv = true;
	gameState = NEW;
	viewState = SHORT;
	bestMove = "";
	goDepthInstruction = "go depth 8";

	hash_value = 512;
	threads_value = 2;
	multiPV_value = 4;

	instruction.push("setoption name Hash value " + std::to_string(hash_value));
	instruction.push("setoption name Threads value " + std::to_string(threads_value));
	instruction.push("setoption name MultiPV value " + std::to_string(multiPV_value));

	waitForReadyOK = false;


	start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::vector<std::string> Parser::getVecMove(std::string moveList)
{
	std::istringstream iss(moveList);
	std::vector<std::string> vecMove((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());

	return vecMove;
}

std::vector<std::string> Parser::parse(std::string subject)
{
	std::vector<std::string> vecInstruction;

	// ================================================================================
	// UCI engine output
	// ================================================================================
	const std::regex rexInfo(
			R"(^info depth (\d+) seldepth (\d+) multipv (\d+) score ([a-z]+) (\-?\d+) \w* ?nodes (\d+) nps (\d+)(?: hashfull |)(\d+|) tbhits (\d+) time (\d+) pv (.*))");
	const std::regex rexInfoCurrentMove(R"(info depth \d+ currmove \w+ currmovenumber \d+)");
	const std::regex rexBestMove(R"(bestmove (\w+) ponder (\w+))");
	const std::regex rexReadyOK(R"(.*readyok.*)");

	std::smatch match;


	try
	{
		// info from chess engine
		if (std::regex_search(subject, match, rexInfo) && match.size() == 12)
		{
			ea.depth = std::stoi(match[1]);
			ea.seldepth = std::stoi(match[2]);
			ea.multipv = std::stoi(match[3]);
			ea.score_type = match[4];
			ea.score = std::stoi(match[5]);
			ea.nodes = std::stoi(match[6]);
			ea.nps = std::stoi(match[7]);

			if (match[8].length() == 0)
				ea.hashfull = 0;
			else
				ea.hashfull = stoi(match[8]);

			ea.tbhits = std::stoi(match[9]);
			ea.time = std::stoi(match[10]);
			ea.moveList = match[11];

			if (getVecMove(current_moveList).size() % 2 == 1)
				ea.score = -ea.score;

			if (new_ply)
			{
				new_ply = false;
				new_depth = false;
				new_pv = false;
				current_ply ++;
				current_depth = 0;
				current_pv = 0;
				current_pv_count = 0;
				game.vecPly.push_back(Ply());
				game.vecPly.back().ply_nr = current_ply;
				game.vecPly.back().moveList = current_moveList;

				for (size_t i = 0; i < 4; i++)
				{
					game.vecPly.back().vecEA.push_back(EngineAnalysis());
				}
				game.vecPly.back().elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - start_time;
			}

			/*

			if (current_depth != ea.depth)
				new_depth = true;

			if (new_depth)
			{
				new_depth = false;
				new_pv = false;
				current_depth = ea.depth;
				current_pv = 0;
				game.vecPly.back().vecEA.clear();
			}

			if (current_pv < ea.multipv)
				new_pv = true;

			if (new_pv)
			{
				new_pv = false;
				current_pv = ea.multipv;
				game.vecPly.back().vecEA.push_back(ea);
				// sort the variants in alphabetical order confuses sometimes, better to
				// have it in the order 'best move first'.
				// std::sort(game.vecPly.back().vecEA.begin(), game.vecPly.back().vecEA.end());

				if (current_pv_count < game.vecPly.back().vecEA.size())
					current_pv_count++;

				// print eval and board
				if (viewState == SHORT && game.vecPly.back().vecEA.size() == current_pv_count)
				{
					// ##### prints too much info.
					// printShortInfo(game.vecPly.back());
				}
			}
			*/

			game.vecPly.back().vecEA[ea.multipv - 1] = ea;



		}
		// "readyok"
		// as soon as the engine writes "readyok" the userStdOut queue can be parsed again.
		else if (std::regex_search(subject, match, rexReadyOK))
		{
			waitForReadyOK = false;
			// std::cout << "READYOK!" << std::endl;
		}
		// info from chess engine
		else if (std::regex_search(subject, match, rexInfoCurrentMove))
		{
			// do nothing
		}
		// ================================================================================
		// Best Move
		// ================================================================================
		else if (std::regex_search(subject, match, rexBestMove) && match.size() == 3)
		{
			bestMove = match[1];

			// auto game
			if (gameState == AUTO)
			{
				new_ply = true;
				current_moveList = current_moveList + bestMove + " ";
				std::string instrMoveList = "position startpos moves " + current_moveList;
				vecInstruction.push_back(instrMoveList);
				vecInstruction.push_back(goDepthInstruction);
			}
		}
		else
		{
			// do nothing for the moment
		}
	}
	catch (std::regex_error& e)
	{
		// Syntax error in the regular expression
		std::cout << "regex not valid" << std::endl;
		std::cout << e.what() << std::endl;
	}

	return vecInstruction;
}


std::vector<std::string> Parser::parseUser(std::string subject)
{
	std::vector<std::string> vecInstruction;

	// ================================================================================
	// User Commands
	// ================================================================================
	const std::regex rexGameState(R"(game (\w+)(?: |)(\w+|))");
	const std::regex rexViewState(R"(view (\w+)(?: |)(\w+|))");

	// from chessCom
	const std::regex rexChessComInfo(R"rex(chesscom info)rex");
	const std::regex rexChessComNewMoveList(R"rex(chesscom movelist "([\w ]+))rex");

	const std::regex rexIsReady(R"(isready)");

	std::smatch match;

	try
	{
		// Game State
		if (std::regex_search(subject, match, rexGameState))
		{
			std::string subCommand = match[1];
			std::string param1 = match[2];

			if (subCommand == "info")
				printInfo();
			else if (subCommand == "new")
			{
				gameState = NEW;
				vecInstruction.push_back("stop");
				vecInstruction.push_back("position startpos");
				vecInstruction.push_back("ucinewgame");
				init();
				std::cout << ">> game new" << std::endl;
				start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			}
			else if (subCommand == "auto")
			{
				gameState = AUTO;
				std::cout << ">> game auto" << std::endl;
				vecInstruction.push_back(goDepthInstruction);
			}
			else if (subCommand == "stop")
			{
				gameState = STOP;
				std::cout << ">> game stop" << std::endl;
			}
			else if (subCommand == "move")
			{
				// do not add moves when AUTO mode is on
				if (gameState != AUTO)
				{
					current_moveList = current_moveList + param1 + " ";
					std::string instrMoveList = "position startpos moves " + current_moveList;
					vecInstruction.push_back(instrMoveList);

					new_ply = true;
				}
			}
			else if (subCommand == "depth")
			{
				goDepthInstruction = "go depth " + param1;
			}
			else if (subCommand == "multipv")
			{
				multiPV_value = std::stoi(param1);
				instruction.push("setoption name MultiPV value " + std::to_string(multiPV_value));
			}
			else if (subCommand == "flip")
			{
				game.flip = ! game.flip;
			}
			else
				std::cout << ">> game <unknown command>" << std::endl;
		}
		// View State
		else if (std::regex_search(subject, match, rexViewState))
		{
			std::string subCommand = match[1];
			std::string param1 = match[2];

			if (subCommand == "eval")
			{
				if (param1 == "none")
					viewState = NONE;
				else if (param1 == "short")
					viewState = SHORT;
				else
					std::cout << ">> view eval <unknown command>" << std::endl;
			}
			else
				std::cout << ">> view <unknown command>" << std::endl;
		}
		// "isready"
		// set waitForReadyOK = tue in order to no longer parse the userStdOut queue in operator()(), till the engine is ready
		else if (std::regex_search(subject, match, rexIsReady))
		{
			waitForReadyOK = true;
			vecInstruction.push_back(subject);
			// std::cout << "ISREADY?" << std::endl;
		}
		// chessCom
		// get info from chesscom
		else if (std::regex_search(subject, match, rexChessComInfo))
		{
			// do nothing for the moment
		}
		// automatically get movelist from chess.com
		else if (std::regex_search(subject, match, rexChessComNewMoveList))
		{
			new_ply = true;
			const std::string chessComMoveList = match[1];
			const std::string setMoveList = "position startpos moves " + chessComMoveList;
			userStdOut.push(std::vector<std::string> { "stop", "isready", setMoveList, "isready", "go infinite" });
			current_moveList = chessComMoveList;

		}
		// just forward the user input to the instruction queue, which will be parsed by the chess engine
		else
		{
			vecInstruction.push_back(subject);
		}
	}
	catch (std::regex_error& e)
	{
		// Syntax error in the regular expression
		std::cout << "regex not valid" << std::endl;
		std::cout << e.what() << std::endl;
	}

	return vecInstruction;
}


void Parser::printCurrentParams()
{
	std::cout << "current_ply = " << current_ply << std::endl;
	std::cout << "current_moveList = " << current_moveList << std::endl;
	std::cout << "current_pv = " << current_pv << std::endl;
	std::cout << "current_depth = " << current_depth << std::endl;
}

void Parser::printShortInfo(Ply ply)
{
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << ply.moveList << std::endl;
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
	for (auto& ea : ply.vecEA)
	{
		std::cout << ea.depth << " " << ea.score_type << " " << ea.score << "   " << ea.moveList << std::endl;
	}
	std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
}

void Parser::printInfo()
{

	if (game.vecPly.size() != 0)
	{
		const auto ply = game.vecPly.back();
		std::cout << " ply_nr=" << ply.ply_nr << "  current_moveList=\"" << ply.moveList << "\"" << std::endl;
		std::cout << " elapsed_time=" << ply.elapsed_time << std::endl;
		std::cout << "----------------------------------------------------------------------------------" << std::endl;

		for (auto& ea : ply.vecEA)
		{
			std::cout << "------------------------------------------------------------------------" << std::endl;
			std::cout << "      depth      " << ea.depth << std::endl;
			// std::cout << "      seldepth   " << ea.seldepth << std::endl;
			std::cout << "      multipv    " << ea.multipv << std::endl;
			std::cout << "      score_type " << ea.score_type << std::endl;
			std::cout << "      score      " << ea.score << std::endl;
			// std::cout << "      nodes      " << ea.nodes << std::endl;
			std::cout << "      nps        " << ea.nps << std::endl;
			// std::cout << "      hashfull   " << ea.hashfull << std::endl;
			// std::cout << "      tbhits     " << ea.tbhits << std::endl;
			// std::cout << "      time       " << ea.time << std::endl;
			std::cout << "      moveList   " << ea.moveList << std::endl;
			std::cout << "------------------------------------------------------------------------" << std::endl;
		}
		std::cout << "----------------------------------------------------------------------------------" << std::endl;
	}
	std::cout << "----------------------------------------------------------------------------------" << std::endl;
	std::cout << " gameState=" << gameState << "  bestMove=" << bestMove  << "  depth=" << goDepthInstruction << "  multiPV=" << multiPV_value << std::endl;
	std::cout << "----------------------------------------------------------------------------------" << std::endl;
}


void Parser::operator()()
{
	std::string line;
	unsigned int notReadyOut = 0;

	while (true)
	{
		// parse output from chess engine queue till the queue is empty. this is necessary, because otherwise there is a huge backlog
		// in the stdOut queue (because of the wait in order to not hook on the CPU),
		// which will have a negative impact in parsing the stdOut queue in time.
		while (true)
		{
			line = stdOut.pop();
			if (line != "NULL")
			{
				// get vector of instructions and push it to the instruction queue
				instruction.push(parse(line));
			}
			else
			{
				break;
			}
		}

		// parse output from user queue
		// only accept user input if chess engine is ready
		if (! waitForReadyOK)
		{
			// forward user input directly to instruction queue
			line = userStdOut.pop();
			if (line != "NULL")
			{
				std::vector<std::string> vecInstruction = parseUser(line);

				for (auto& instr : vecInstruction)
				{
					instruction.push(instr);
				}
			vecInstruction.clear();
			}
		}
		else
		{
			// no not print too much this text, ...
			if (notReadyOut % 100 == 0)
			{
				std::cout << "ENGINE NOT READY: waiting, ..." << std::endl;
			}
			notReadyOut++;

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
	}
}
