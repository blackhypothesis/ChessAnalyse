#include "PVboards.h"

PVboards::PVboards() :
		numberBoards(4), fieldSize(40.0f), numberInfo(3)
{
	for (size_t i = 0; i < numberBoards; i++)
	{
		vecBoard.push_back(ChessBoard());

		std::vector<TextInput> vT;
		for (size_t info = 0; info < numberInfo; info++)
		{
			vT.push_back(TextInput());
		}
		vecText.push_back(vT);
	}


	adjust();
}

void PVboards::adjust()
{
	float boardSize = 8 * fieldSize;

	for (size_t y = 0; y < 2; y++)
	{
		for (size_t x = 0; x < 2; x++)
		{
			unsigned int idx = 2 * y + x;
			vecBoard[idx].setPosition( { 15.0f + x * (boardSize + 50.0f), 15.0f + y * (boardSize + 50.0f) });
			vecBoard[idx].setFieldSize(fieldSize);

			for (size_t info = 0; info < numberInfo; info++)
			{
				vecText[idx][info].setPosition( { 15.0f + x * (boardSize + 50.0f) + 120 * info, 20.0f + y * (boardSize + 50.0f) + boardSize });
				vecText[idx][info].setFontSize(14);
				vecText[idx][info].setLength(12);
				vecText[idx][info].setInput("");
			}
		}
	}
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
				if (game.vecPly.back().vecEA[i].score > 100)
					vecBoard[i].setColors( { 200, 200, 200 }, { 58, 108, 189 });
				else if (game.vecPly.back().vecEA[i].score > 0)
					vecBoard[i].setColors( { 200, 200, 200 }, { 66, 166, 78 });
				else if (game.vecPly.back().vecEA[i].score > -100)
					vecBoard[i].setColors( { 200, 200, 200 }, { 240, 232, 7 });
				else
					vecBoard[i].setColors( { 200, 200, 200 }, { 212, 71, 42 });

				vecBoard[i].setMoves(game.vecPly.back().moveList);
				vecBoard[i].setVariant(game.vecPly.back().vecEA[i].moveList);
				vecBoard[i].setScore(game.vecPly.back().vecEA[i].score);
				vecBoard[i].updateMoves();
				vecBoard[i].setAnimateVariation(true);

				std::stringstream score;
				std::string moveList = game.vecPly.back().vecEA[i].moveList;
				score << std::setw(6) << std::setprecision(2)<< (float)(game.vecPly.back().vecEA[i].score) / 100;
				moveList = moveList.substr(0, 10);
				int depth = game.vecPly.back().vecEA[i].depth;

				std::vector<std::string> infoMsg{ moveList, score.str(), std::to_string(depth) };

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

void PVboards::draw(sf::RenderWindow &target) const
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
}
