#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

using namespace std;


//Prints welcome menu
void Game::printWelcomeMenu()
{
	sf::RenderWindow window(sf::VideoMode(800, 600),
		"HelloSFML", sf::Style::Default);

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	//Prints welcome message
	sf::Text text;
	text.setCharacterSize(40);
	text.setOrigin(0, 20);
	text.setFont(font);
	text.setPosition(200, 75);
	text.setFillColor(sf::Color::White);
	text.setString("WELCOME TO FROGGER");

	//Prints "Frog"
	sf::RectangleShape rectangle(sf::Vector2f(200, 200));
	rectangle.setFillColor(sf::Color(0, 255, 0));
	rectangle.setPosition(300, 200);

	while (window.isOpen())
	{
		//Prints difficulty and mode menus
		window.clear();
		window.draw(rectangle);
		window.draw(text);
		window.display();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				window.close();
			}
		}
	}
}
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void Game::printDifficultyMenu()
{
	int difficulty = 2;
	sf::RenderWindow window(sf::VideoMode(800, 600),
		"HelloSFML", sf::Style::Default);

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	for (int ii = 1; ii < 5; ii++)
	{
		for (int jj = 75; jj < 675; jj = jj + 150)
		{
			sf::Text ii;
			ii.setCharacterSize(30);
			ii.setOrigin(0, 15);
			ii.setFont(font);
			ii.setPosition(200, jj);
			ii.setFillColor(sf::Color::Black);
			if (jj = 75)
				ii.setString("SELECT DIFFICULTY:");
			else if (jj = 225)
				ii.setString("EASY");
			else if (jj = 375)
				ii.setString("NORMAL");
			else
				ii.setString("HARD");


			/*sf::Text text2;
			text2.setCharacterSize(30);
			text2.setOrigin(0, 15);
			text2.setFont(font);
			text2.setPosition(200, 225);
			text2.setFillColor(sf::Color::Black);
			text2.setString("EASY");

			sf::Text text3;
			text3.setCharacterSize(30);
			text3.setOrigin(0, 15);
			text3.setFont(font);
			text3.setPosition(200, 375);
			text3.setFillColor(sf::Color::Black);
			text3.setString("NORMAL");

			sf::Text text4;
			text4.setCharacterSize(30);
			text4.setOrigin(0, 15);
			text4.setFont(font);
			text4.setPosition(200, 525);
			text4.setFillColor(sf::Color::Black);
			text4.setString("HARD");*/

			sf::CircleShape circle(20);
			circle.setOrigin(20, 20);
			circle.setFillColor(sf::Color(255, 255, 255));
			circle.setOutlineThickness(-5);
			circle.setOutlineColor(sf::Color(0, 0, 0));
			circle.setPosition(100, 375);

			sf::RectangleShape rectangle1(sf::Vector2f(800, 150));
			rectangle1.setFillColor(sf::Color(255, 255, 255));
			rectangle1.setOutlineThickness(-20);
			rectangle1.setOutlineColor(sf::Color(0, 0, 0));

			sf::RectangleShape rectangle2(sf::Vector2f(800, 150));
			rectangle2.setFillColor(sf::Color(255, 255, 255));
			rectangle2.setOutlineThickness(-20);
			rectangle2.setOutlineColor(sf::Color(0, 0, 0));
			rectangle2.setPosition(0, 150);

			sf::RectangleShape rectangle3(sf::Vector2f(800, 150));
			rectangle3.setFillColor(sf::Color(255, 255, 255));
			rectangle3.setOutlineThickness(-20);
			rectangle3.setOutlineColor(sf::Color(0, 0, 0));
			rectangle3.setPosition(0, 300);

			sf::RectangleShape rectangle4(sf::Vector2f(800, 150));
			rectangle4.setFillColor(sf::Color(255, 255, 255));
			rectangle4.setOutlineThickness(-20);
			rectangle4.setOutlineColor(sf::Color(0, 0, 0));
			rectangle4.setPosition(0, 450);


			while (window.isOpen())
			{
				//Prints difficulty and mode menus
				//window.clear();
				window.draw(rectangle1);
				window.draw(rectangle2);
				window.draw(rectangle3);
				window.draw(rectangle4);
				window.draw(ii);
				window.draw(circle);
				window.display();
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						window.close();
					}
					//Changes difficulty
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					{
						if (difficulty > 1)
						{
							circle.move(0, -150);
							difficulty -= 1;
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					{
						if (difficulty < 3)
						{
							circle.move(0, 150);
							difficulty += 1;
						}
					}
					//Closes window
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
					{
						window.close();
					}
				}
			}
		}
	}
	this->difficulty = difficulty;
}
/*

//Prints Mode Menu
int printModeMenu(int mode)
{
	int mode2 = 2;
	sf::RenderWindow window(sf::VideoMode(800, 600),
		"HelloSFML", sf::Style::Default);

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	//Prints mode text
	sf::Text text1;
	text1.setCharacterSize(30);
	text1.setOrigin(0, 15);
	text1.setFont(font);
	text1.setPosition(200, 75);
	text1.setFillColor(sf::Color::Black);
	text1.setString("SELECT MODE:");

	sf::Text text2;
	text2.setCharacterSize(30);
	text2.setOrigin(0, 15);
	text2.setFont(font);
	text2.setPosition(200, 225);
	text2.setFillColor(sf::Color::Black);
	text2.setString("TIMED");

	sf::Text text3;
	text3.setCharacterSize(30);
	text3.setOrigin(0, 15);
	text3.setFont(font);
	text3.setPosition(200, 375);
	text3.setFillColor(sf::Color::Black);
	text3.setString("CLASSIC");

	sf::Text text4;
	text4.setCharacterSize(30);
	text4.setOrigin(0, 15);
	text4.setFont(font);
	text4.setPosition(200, 525);
	text4.setFillColor(sf::Color::Black);
	text4.setString("INFINITE");

	//Visual aid for user
	sf::CircleShape circle(20);
	circle.setOrigin(20, 20);
	circle.setFillColor(sf::Color(255, 255, 255));
	circle.setOutlineThickness(-5);
	circle.setOutlineColor(sf::Color(0, 0, 0));
	circle.setPosition(100, 375);

	//Divides menu into quadrants
	sf::RectangleShape rectangle1(sf::Vector2f(800, 150));
	rectangle1.setFillColor(sf::Color(255, 255, 255));
	rectangle1.setOutlineThickness(-20);
	rectangle1.setOutlineColor(sf::Color(0, 0, 0));

	sf::RectangleShape rectangle2(sf::Vector2f(800, 150));
	rectangle2.setFillColor(sf::Color(255, 255, 255));
	rectangle2.setOutlineThickness(-20);
	rectangle2.setOutlineColor(sf::Color(0, 0, 0));
	rectangle2.setPosition(0, 150);

	sf::RectangleShape rectangle3(sf::Vector2f(800, 150));
	rectangle3.setFillColor(sf::Color(255, 255, 255));
	rectangle3.setOutlineThickness(-20);
	rectangle3.setOutlineColor(sf::Color(0, 0, 0));
	rectangle3.setPosition(0, 300);

	sf::RectangleShape rectangle4(sf::Vector2f(800, 150));
	rectangle4.setFillColor(sf::Color(255, 255, 255));
	rectangle4.setOutlineThickness(-20);
	rectangle4.setOutlineColor(sf::Color(0, 0, 0));
	rectangle4.setPosition(0, 450);


	while (window.isOpen())
	{
		window.clear();
		window.draw(rectangle1);
		window.draw(rectangle2);
		window.draw(rectangle3);
		window.draw(rectangle4);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.draw(text4);
		window.draw(circle);
		window.display();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			//Changes mode
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (mode > 1)
				{
					circle.move(0, -150);
					mode -= 1;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (mode < 3)
				{
					circle.move(0, 150);
					mode += 1;
				}
			}
			//Closes window
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				window.close();
			}
		}
	}
}
*/

//Arguments to determine loss?
//void printLoserScreen()
int printLoserScreen()
{
	int option = 2;
	sf::RenderWindow window(sf::VideoMode(800, 800),
		"HelloSFML", sf::Style::Default);

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	sf::Text text;
	text.setCharacterSize(40);
	text.setOrigin(0, 20);
	text.setFont(font);
	text.setPosition(300, 100);
	text.setFillColor(sf::Color::White);
	text.setString("YOU LOSE!");

	sf::Text text1;
	text1.setCharacterSize(40);
	text1.setOrigin(0, 20);
	text1.setFont(font);
	text1.setPosition(300, 500);
	text1.setFillColor(sf::Color::White);
	text1.setString("MENU");

	sf::Text text2;
	text2.setCharacterSize(40);
	text2.setOrigin(0, 20);
	text2.setFont(font);
	text2.setPosition(300, 600);
	text2.setFillColor(sf::Color::White);
	text2.setString("REPLAY");

	sf::Text text3;
	text3.setCharacterSize(40);
	text3.setOrigin(0, 20);
	text3.setFont(font);
	text3.setPosition(300, 700);
	text3.setFillColor(sf::Color::White);
	text3.setString("EXIT");

	sf::CircleShape circle(20);
	circle.setOrigin(20, 20);
	circle.setFillColor(sf::Color(0, 0, 0));
	circle.setOutlineThickness(-5);
	circle.setOutlineColor(sf::Color(255, 255, 255));
	circle.setPosition(200, 600);

	sf::RectangleShape rectangle(sf::Vector2f(200, 200));
	rectangle.setFillColor(sf::Color(255, 0, 0));
	rectangle.setPosition(300, 200);

	while (window.isOpen())
	{
		window.clear();
		window.draw(text);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.draw(circle);
		window.draw(rectangle);
		window.display();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (option > 1)
				{
					circle.move(0, -100);
					option -= 1;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (option < 3)
				{
					circle.move(0, 100);
					option += 1;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				window.close();
			}
		}
	}
	return option;
}


//Arguments to determine win?
//void printWinnerScreen()
int printWinnerScreen()
{
	int option = 2;
	sf::RenderWindow window(sf::VideoMode(800, 800),
		"HelloSFML", sf::Style::Default);

	sf::Font font;
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	sf::Text text;
	text.setCharacterSize(40);
	text.setOrigin(0, 20);
	text.setFont(font);
	text.setPosition(300, 100);
	text.setFillColor(sf::Color::White);
	text.setString("YOU WIN!");

	sf::Text text1;
	text1.setCharacterSize(40);
	text1.setOrigin(0, 20);
	text1.setFont(font);
	text1.setPosition(300, 500);
	text1.setFillColor(sf::Color::White);
	text1.setString("MENU");

	sf::Text text2;
	text2.setCharacterSize(40);
	text2.setOrigin(0, 20);
	text2.setFont(font);
	text2.setPosition(300, 600);
	text2.setFillColor(sf::Color::White);
	text2.setString("REPLAY");

	sf::Text text3;
	text3.setCharacterSize(40);
	text3.setOrigin(0, 20);
	text3.setFont(font);
	text3.setPosition(300, 700);
	text3.setFillColor(sf::Color::White);
	text3.setString("EXIT");

	sf::CircleShape circle(20);
	circle.setOrigin(20, 20);
	circle.setFillColor(sf::Color(0, 0, 0));
	circle.setOutlineThickness(-5);
	circle.setOutlineColor(sf::Color(255, 255, 255));
	circle.setPosition(200, 600);

	sf::RectangleShape rectangle(sf::Vector2f(200, 200));
	rectangle.setFillColor(sf::Color(0, 255, 0));
	rectangle.setPosition(300, 200);

	while (window.isOpen())
	{
		window.clear();
		window.draw(text);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.draw(circle);
		window.draw(rectangle);
		window.display();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				if (option > 1)
				{
					circle.move(0, -100);
					option -= 1;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				if (option < 3)
				{
					circle.move(0, 100);
					option += 1;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				window.close();
			}
		}
	}
	return option;
}