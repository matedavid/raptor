#include <iostream>

#include <SFML/Graphics.hpp>

int main_2(int argc, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(512, 512), "Raptor", sf::Style::Default);

  sf::Font font;
  if (not font.loadFromFile("../src/gui/Roboto/Roboto-Regular.ttf"))
  {
  }

  sf::Text text;
  text.setString("Hello world");
  text.setFont(font);
  text.setCharacterSize(16);
  text.setFillColor(sf::Color::Black);

  sf::Vector2 pos = sf::Vector2(0,1);
  text.setPosition(50.0, 100.0);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    window.clear(sf::Color::White);

    window.draw(text);

    window.display();
  }
}
