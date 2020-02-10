#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "TextInput.h"
#include "ChessBoard.h"


int main(int argc, char* argv[])
{
	std::string cwp(argv[0]);

	int nFPS = 10;
	sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Chess Analyse");
	window.setFramerateLimit(nFPS);

	TextInput text = TextInput({ 15.0f, 550.0f });
	std::string input;

	ChessBoard cb;

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
		}

		input = text.getInput();

		if (input.size() > 0)
		{
			std::cout << "input = " << input << std::endl;
		}

		window.clear();
		cb.draw(window);
		text.draw(window);
		window.display();

		if (!nFPS--)
		{
			nFPS = (int) (1000000 / elapsed.asMicroseconds());
			std::cout << "FPS: " << nFPS << std::endl;
		}
	}

	return 0;
}
