#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Parser.h"
#include "ThreadSaveQueue.h"
#include "ProcessInOut.h"
#include "ChessCom.h"

#include "TextInput.h"
#include "ChessBoard.h"
#include "PVboards.h"

int main(int argc, char *argv[])
{
	std::string cwp(argv[0]);

	const std::string chessEnginePath = "/home/marcel/workspace/ChessAnalyse/assets/bin/stockfish-bmi2";
	ThreadSaveQueue stdIn("stdIn");
	ThreadSaveQueue stdOut("stdOut");
	ThreadSaveQueue instruction("instruction");
	ThreadSaveQueue userStdOut("userStdOut");

	int nFPS = 10;
	sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(1000, 950), "Chess Analyse");
	window.setFramerateLimit(nFPS);

	TextInput text = TextInput(sf::Vector2f(15.0f, 900.0f));
	std::string input;

	PVboards pvBoards;

	Game game;

	ChildProcInOut chessEngine = ChildProcInOut(chessEnginePath, stdIn, stdOut, instruction);
	chessEngine.execChild();

	Parser parser(stdOut, userStdOut, instruction, game);
	std::thread parserThread(parser);

	ChessCom cc(userStdOut);
	std::thread ccThread(cc);

	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::TextEntered)
				text.processEvent(event);

			if (event.type == sf::Event::MouseButtonPressed)
			{
			    if (event.mouseButton.button == sf::Mouse::Right)
			    {
			    	userStdOut.push("game flip");
			    }
			}
		}

		input = text.getInput();

		if (input.size() > 0)
		{
			userStdOut.push(input);
		}

		pvBoards.update(game);

		window.clear();
		pvBoards.draw(window);
		text.draw(window);

		window.display();

		if (!nFPS--)
		{
			nFPS = (int) (1000000 / elapsed.asMicroseconds());
			// std::cout << "FPS: " << nFPS << std::endl;
		}
	}

	return 0;
}
