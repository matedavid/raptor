#include <iostream>

#include <SFML/Graphics.hpp>

int main_2(int argc, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(1024, 512), "Raptor", sf::Style::Default);

  sf::Font font;
  if (not font.loadFromFile("../src/gui/Roboto/Roboto-Regular.ttf"))
  {
  }

  sf::Text text;
  text.setString("Hello world");
  text.setFont(font);
  text.setCharacterSize(20);
  text.setFillColor(sf::Color::Black);
  text.setPosition(8, 16);

  sf::VertexArray line(sf::Lines, 2);
  line[0].position = sf::Vector2f(8, 16);
  line[0].color = sf::Color::Red;
  line[1].position = sf::Vector2f(1016, 16);
  line[1].color = sf::Color::Red;

  sf::VertexArray line2(sf::Lines, 2);
  line2[0].position = sf::Vector2f(8, 16+text.getCharacterSize()*1.2);
  line2[0].color = sf::Color::Red;
  line2[1].position = sf::Vector2f(1016, 16+text.getCharacterSize()*1.2);
  line2[1].color = sf::Color::Red;



  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::Resized)
        std::cout << "Width: " << event.size.width << " Height: " << event.size.height << std::endl;
    }

    window.clear(sf::Color::White);

    window.draw(text);
    window.draw(line);
    window.draw(line2);

    window.display();
  }
}
