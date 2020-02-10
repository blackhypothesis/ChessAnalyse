#include "ChessBoard.h"

ChessBoard::ChessBoard() :
	offset({ 20.0f, 20.0f }),
	fieldSize(50.0f),
	whiteField(200, 200, 200),
	blackField(120, 120, 220),
	flip(false),
	highlightLastMove(true),
	pieceSize(320.0f),
	piecePath("/home/marcel/workspace/ChessAnalyse/assets/merida/320"),

	aPieceImage({ "BlackKing.png", "BlackQueen.png", "BlackRook.png", "BlackBishop.png", "BlackKnight.png", "BlackPawn.png",
			 "empty.png",
			 "WhitePawn.png", "WhiteKnight.png", "WhiteBishop.png", "WhiteRook.png", "WhiteQueen.png", "WhiteKing.png" }),
	iCurrentPly(vecVariant.begin()),
	score(0),
	x1(0), y1(0), x2(0), y2(0)
{
	// load texture for each chess piece
	for (size_t i = 0; i < aPieceTexture.size(); i++)
	{
		aPieceTexture[i].loadFromFile(piecePath + "/" + aPieceImage[i]);
		aPieceTexture[i].setSmooth(true);
	}

	setChessStartPosition();
	updateChessBoard();
}

void ChessBoard::setChessStartPosition()
{
	// set the initial board position
	board[0][0].piece = WR; board[1][0].piece = WN; board[2][0].piece = WB; board[3][0].piece = WQ;
	board[4][0].piece = WK; board[5][0].piece = WB; board[6][0].piece = WN; board[7][0].piece = WR;
	for (size_t x = 0; x < board[1].size(); x++)
		board[x][1].piece = WP;
	for (size_t y = 2; y < 6; y++)
	{
		for (size_t x = 0; x < board[y].size(); x++)
		{
			board[x][y].piece = EMPTY;
		}
	}
	for (size_t x = 0; x < board[6].size(); x++)
		board[x][6].piece = BP;
	board[0][7].piece = BR; board[1][7].piece = BN; board[2][7].piece = BB; board[3][7].piece = BQ;
	board[4][7].piece = BK; board[5][7].piece = BB; board[6][7].piece = BN; board[7][7].piece = BR;

	for (size_t y = 0; y < board.size(); y++)
		for (size_t x = 0; x < board[y].size(); x++)
			board[x][y].highlight = 0;
}


void ChessBoard::updateChessBoard()
{
	// scorebar (min -4.00 max +4.00)
	int scoreAdjust = score;

	if (score > 400)
		scoreAdjust = 400;
	else if (score < -400)
		scoreAdjust = -400;

	sf::Vector2f posScoreRect = offset + sf::Vector2f(8.2f * fieldSize, 0);
	sf::Vector2f sizeScoreRect = sf::Vector2f(0.3f * fieldSize, 8.0f * fieldSize);

	scoreRect.setPosition(posScoreRect);
	scoreRect.setSize(sizeScoreRect);
	scoreRect.setFillColor(sf::Color(60, 60, 60));

	float scoreHight = ((scoreAdjust / 100.0f) + 4.0f) * fieldSize;

	sf::Vector2f posScoreBar;
	sf::Vector2f sizeScoreBar;
	scoreBar.setFillColor(sf::Color(255, 255, 255));


	// set the color for each field and the appropriate chess piece on this field
	for (size_t y = 0; y < board.size(); y++)
	{
		for (size_t x = 0; x < board[y].size(); x++)
		{
			float posX = offset.x + x * fieldSize;
			float posY = offset.y + (7 - y) * fieldSize;

			sf::Color fieldColor;
			if (x % 2 == 0 && y % 2 == 0)
				fieldColor = blackField;
			else if (x % 2 != 0 && y % 2 != 0)
				fieldColor = blackField;
			else
				fieldColor = whiteField;

			board[x][y].field.setFillColor(fieldColor);
			board[x][y].field.setPosition({ posX, posY });
			board[x][y].field.setSize({ fieldSize, fieldSize });

			board[x][y].pieceSprite.setPosition({ posX, posY });
			board[x][y].pieceSprite.setScale(fieldSize / pieceSize, fieldSize / pieceSize);
		}
	}

	for (size_t y = 0; y < board.size(); y++)
	{
		for (size_t x = 0; x < board[y].size(); x++)
		{
			if (!flip)
			{
				board[x][y].pieceSprite.setTexture(aPieceTexture[board[x][y].piece]);
				if (board[x][y].highlight > 0)
				{
					auto h = board[x][y].highlight;
					board[x][y].field.setFillColor(sf::Color(144 + h, 22 + h, 172 + h));
				}
			}
			else
			{
				board[7 - x][7 - y].pieceSprite.setTexture(aPieceTexture[board[x][y].piece]);
				if (board[x][y].highlight > 0)
				{
					auto h = board[x][y].highlight;
					board[7 - x][7 - y].field.setFillColor(sf::Color(144 + h, 22 + h, 172 + h));
				}
			}
		}
	}

	if (!flip)
	{
		posScoreBar = offset + sf::Vector2f(8.2f * fieldSize, 8.0f * fieldSize - scoreHight);
		sizeScoreBar = sf::Vector2f(0.3f * fieldSize, scoreHight);
		scoreBar.setPosition(posScoreBar);
		scoreBar.setSize(sizeScoreBar);
	}
	else
	{
		posScoreBar = offset + sf::Vector2f(8.2f * fieldSize, 0.0f);
		sizeScoreBar = sf::Vector2f(0.3f * fieldSize, scoreHight);
		scoreBar.setPosition(posScoreBar);
		scoreBar.setSize(sizeScoreBar);
	}
}


void ChessBoard::setMoves(std::string const moveList)
{
	vecMove = getVecMove(moveList);
}


void ChessBoard::setVariant(std::string const variant)
{
	vecVariant = getVecMove(variant);
	iCurrentPly = vecVariant.begin();
}


void ChessBoard::setScore(const int score)
{
	this->score = score;
}


void ChessBoard::updateMoves()
{
	setChessStartPosition();
	auto vecMovesAndVariant = vecMove;

	if (vecVariant.size() > 1)
	{

		/*
		for (size_t i = 0; i < 2; i++)
		{
			vecMovesAndVariant.push_back(vecVariant[i]);
			x1 = (int)vecVariant[i][0] - 'a';
			y1 = vecVariant[i][1] - '0' - 1;
			x2 = (int)vecVariant[i][2] - 'a';
			y2 = vecVariant[i][3] - '0' - 1;

			if (highlightLastMove == true && vecMove.size() > 0)
			{
				board[x1][y1].highlight = 1 + 70 * i;
				board[x2][y2].highlight = 1 + 70 * i;
			}
		}
		vecMovesAndVariant.pop_back();
		*/

		for (std::vector<std::string>::iterator it = vecVariant.begin(); it <= iCurrentPly; it++)
		{
			vecMovesAndVariant.push_back(*it);

			const int nrPly = iCurrentPly - it;
			if (highlightLastMove == true && nrPly < 1)
			{
				x1 = (int)(*it)[0] - 'a';
				y1 = (*it)[1] - '0' - 1;
				x2 = (int)(*it)[2] - 'a';
				y2 = (*it)[3] - '0' - 1;
				board[x1][y1].highlight = 1 + 70 * nrPly;
				board[x2][y2].highlight = 1 + 70 * nrPly;
			}
		}
	}

	for (auto& move : vecMovesAndVariant)
	{
		x1 = (int)move[0] - 'a';
		y1 = move[1] - '0' - 1;
		x2 = (int)move[2] - 'a';
		y2 = move[3] - '0' - 1;

		PieceType piece = board[x1][y1].piece;
		PieceType promotionPiece = EMPTY;

		// white short rochade
		if (board[x1][y1].piece == WK && x2 == 6 && y2 == 0)
		{
			board[7][0].piece = EMPTY;
			board[5][0].piece = WR;
		}
		// white long rochade
		if (board[x1][y1].piece == WK && x2 == 2 && y2 == 0)
		{
			board[0][0].piece = EMPTY;
			board[3][0].piece = WR;
		}
		// black short rochade
		if (board[x1][y1].piece == BK && x2 == 6 && y2 == 7)
		{
			board[7][7].piece = EMPTY;
			board[5][7].piece = BR;
		}
		// black long rochade
		if (board[x1][y1].piece == BK && x2 == 2 && y2 == 7)
		{
			board[0][7].piece = EMPTY;
			board[3][7].piece = BR;
		}

		// en passant
		// tbd

		// pawn promotion
		if (move.size() == 5)
		{
			if (y2 == 7)
			{
				if (move[4] == 'n')
					promotionPiece = WN;
				else if (move[4] == 'b')
					promotionPiece = WB;
				else if (move[4] == 'r')
					promotionPiece = WR;
				else if (move[4] == 'q')
					promotionPiece = WQ;
			}
			else if (y2 == 0)
			{
				if (move[4] == 'n')
					promotionPiece = BN;
				else if (move[4] == 'b')
					promotionPiece = BB;
				else if (move[4] == 'r')
					promotionPiece = BR;
				else if (move[4] == 'q')
					promotionPiece = BQ;
			}
		}

		if (promotionPiece != EMPTY)
			piece = promotionPiece;
		
		board[x2][y2].piece = piece;
		board[x1][y1].piece = EMPTY;
	}

	updateChessBoard();
}


void ChessBoard::setPosition(sf::Vector2f offset)
{
	this->offset = offset;
	updateChessBoard();
}


void ChessBoard::setFieldSize(float fieldSize)
{
	this->fieldSize = fieldSize;
	updateChessBoard();
}


void ChessBoard::setColors(sf::Color whiteField, sf::Color blackField)
{
	this->whiteField = whiteField;
	this->blackField = blackField;
	updateChessBoard();
}


void ChessBoard::setFlip(bool flip)
{
	this->flip = flip;
	updateChessBoard();
}


void ChessBoard::flipBoard()
{
	flip = !flip;
}


void ChessBoard::setHighlightLastMove(bool highlightLastMove)
{
	this->highlightLastMove = highlightLastMove;
}


void ChessBoard::setAnimateVariation(bool animateVariation)
{
	this->animateVariation = animateVariation;
	iCurrentPly = vecVariant.begin();
}

void ChessBoard::nextPlyAnimateVariation()
{
	if (animateVariation)
	{
		iCurrentPly++;
		if (iCurrentPly == vecVariant.end())
			iCurrentPly = vecVariant.begin();
		updateMoves();
	}
}


std::vector<std::string> ChessBoard::getVecMove(std::string const moveList)
{
	std::istringstream iss(moveList);
	std::vector<std::string> vecMove((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());

	return vecMove;
}


void ChessBoard::draw(sf::RenderTarget& target) const
{
	for (size_t y = 0; y < board.size(); y++)
	{
		for (size_t x = 0; x < board[y].size(); x++)
		{
			target.draw(board[x][y].field);
			target.draw(board[x][y].pieceSprite);
			target.draw(scoreRect);
			target.draw(scoreBar);
		}
	}
}

