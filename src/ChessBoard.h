#pragma once

#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "Arrow.h"


class ChessBoard
{
public:
	ChessBoard();

private:
	sf::Vector2f offset;
	float fieldSize;
	sf::Color whiteField;
	sf::Color blackField;
	bool flip;
	bool highlightLastMove;
	bool animateVariation;

	const float pieceSize;
	enum PieceType { BK, BQ, BR, BB, BN, BP, EMPTY, WP, WN, WB, WR, WQ, WK };
	struct Field
	{
		Field() = default;
		PieceType piece;
		int highlight;
		sf::Sprite pieceSprite;
		sf::RectangleShape field;
	};
	std::array<std::array<Field, 8>, 8> board;
	const std::string piecePath;
	const std::array<std::string, 13> aPieceImage;

	std::array<sf::Texture, 13> aPieceTexture;

	sf::RectangleShape scoreRect;
	sf::RectangleShape scoreBar;

	std::vector<std::string> vecMove;
	std::vector<std::string> vecVariant;
	std::vector<std::string>::iterator iCurrentPly;

	int score;

	unsigned int x1, y1, x2, y2;
	std::vector<Arrow> vecArrow;

public:
	void reset();
	void setChessStartPosition();
	void updateChessBoard();
	void setMoves(std::string);
	void setVariant(std::string);
	void setScore(const int);
	void updateMoves();

	void setPosition(sf::Vector2f);
	void setFieldSize(float);
	void setColors(sf::Color, sf::Color);
	void setFlip(bool);
	void flipBoard();
	void setHighlightLastMove(bool);
	void setAnimateVariation(bool);
	void nextPlyAnimateVariation();
	std::vector<std::string> getVecMove(std::string);
	void draw(sf::RenderTarget&) const;
};

