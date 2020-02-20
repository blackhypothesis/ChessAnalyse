#include "PVboards.h"

PVboards::PVboards(ThreadSaveQueue& inUserStdOut) :
		userStdOut(inUserStdOut),
		numberBoards(4), fieldSize(40.0f), numberInfo(6)
{
	for (size_t i = 0; i < numberBoards; i++)
	{
		ChessBoard cb;
		vecBoard.push_back(ChessBoard());

		std::vector<TextInput> vT;
		for (size_t info = 0; info < numberInfo; info++)
		{
			vT.push_back(TextInput());
		}
		vecText.push_back(vT);
	}

	adjust();

	valueThreads.setParameterName("Threads");
	valueThreads.setMinMaxValue(1, 10);
	valueThreads.setParameterValue(2);
	valueThreads.setStep(1);
	valueThreads.setPosition(sf::Vector2f(300, 822));
 }

void PVboards::adjust()
{
	sf::Vector2f offset = sf::Vector2f(10, 10);
	float boardSize = 8 * fieldSize;

	for (size_t y = 0; y < 2; y++)
	{
		for (size_t x = 0; x < 2; x++)
		{
			unsigned int idx = 2 * y + x;
			vecBoard[idx].setPosition( { offset.x + x * (boardSize + 50.0f), offset.y + y * (boardSize + 90.0f) });
			vecBoard[idx].setFieldSize(fieldSize);

			for (size_t iy = 0; iy < 2; iy++)
			{
				float offsetX = offset.x;

				for (size_t ix = 0; ix < 3; ix++)
				{
					unsigned int idxInfo = 3 * iy + ix;
					vecText[idx][idxInfo].setFontSize(14);
					vecText[idx][idxInfo].setLength(8 + ix * 5);
					// vecText[idx][idxInfo].setPosition( { offset.x + x * (boardSize + 50.0f) + 120 * ix, offset.y + 8 + y * (boardSize + 90.0f) + boardSize  + 30 * iy});


					vecText[idx][idxInfo].setPosition( { offsetX + x * (boardSize + 50.0f), offset.y + 8 + y * (boardSize + 90.0f) + boardSize  + 30 * iy});

					offsetX += vecText[idx][idxInfo].getWidth() + 10;

					vecText[idx][idxInfo].setInput("");
				}
			}
		}
	}
}

std::string PVboards::numWithCommas(const std::string& number)
{
	std::string numWithCommas = number;
	int insertPosition = numWithCommas.length() - 3;
	while (insertPosition > 0) {
	    numWithCommas.insert(insertPosition, ",");
	    insertPosition-=3;
	}
	return numWithCommas;
}


void PVboards::update(Game game)
{
	for (auto& board : vecBoard)
	{
		board.setFlip(game.flip);
	}

	if (game.vecPly.size() > 0)
	{
		for (size_t i = 0; i < vecBoard.size(); i++)
		{
			// game.print();
			if (i < game.vecPly.back().vecEA.size())
			{
				/*
				if (game.vecPly.back().vecEA[i].score > 100)
					vecBoard[i].setColors( { 200, 200, 200 }, { 58, 108, 189 });
				else if (game.vecPly.back().vecEA[i].score > 0)
					vecBoard[i].setColors( { 200, 200, 200 }, { 66, 166, 78 });
				else if (game.vecPly.back().vecEA[i].score > -100)
					vecBoard[i].setColors( { 200, 200, 200 }, { 240, 232, 7 });
				else
					vecBoard[i].setColors( { 200, 200, 200 }, { 212, 71, 42 });
				*/

				vecBoard[i].setMoves(game.vecPly.back().moveList);
				vecBoard[i].setVariant(game.vecPly.back().vecEA[i].moveList);
				vecBoard[i].setScore(game.vecPly.back().vecEA[i].score);
				vecBoard[i].updateMoves();
				vecBoard[i].setAnimateVariation(true);

				std::stringstream score;
				std::string moveList = game.vecPly.back().vecEA[i].moveList;
				if (game.vecPly.back().vecEA[i].score_type == "cp")
				{
					score << std::setw(6) << std::setprecision(3)<< (float)(game.vecPly.back().vecEA[i].score) / 100;
				}
				else
				{
					score << "M " << game.vecPly.back().vecEA[i].score;
				}
				moveList = moveList.substr(0, 15);
				int depth = game.vecPly.back().vecEA[i].depth;
				std::string nps = std::to_string(game.vecPly.back().vecEA[i].nps);
				std::string nodes = std::to_string(game.vecPly.back().vecEA[i].nodes);
				auto ply_nr = game.vecPly.back().ply_nr;
				std::string moveColor = "white";
				std::string move_nr = std::to_string((ply_nr + 1) / 2);
				if (ply_nr % 2 == 0)
					moveColor = "black";
				std::string move_info = moveColor + " " + move_nr;


				std::vector<std::string> infoMsg{ score.str(), move_info, moveList, std::to_string(depth), numWithCommas(nps), numWithCommas(nodes) };

				for (size_t info = 0; info < numberInfo; info++)
				{
					vecText[i][info].setInput(infoMsg[info]);
				}
			}
			else
			{
				vecBoard[i].reset();
			}
		}
	}
	else
	{
		// new game
		for (auto& board : vecBoard)
		{
			board.setChessStartPosition();
			board.updateChessBoard();
		}
	}
}

bool PVboards::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	bool actionReturn = valueThreads.mouseAction(mousePos, buttonPressed, buttonReleased);

	if (actionReturn == true)
	{
		userStdOut.push("game thread " + std::to_string(valueThreads.getParameterValue()));
	}
	return actionReturn;
}


void PVboards::draw(sf::RenderTarget &target) const
{
	for (auto &board : vecBoard)
	{
		board.draw(target);
	}

	for (auto &info : vecText)
	{
		for(auto &i : info)
		{
			i.draw(target);
		}
	}
	valueThreads.draw(target);
}
