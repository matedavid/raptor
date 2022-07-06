#include <iostream>
#include <SFML/Graphics.hpp>

#include "liquid/html/html_document.h"
#include "liquid/renderer/viewport.h"
#include "painter.h"

#include "network/network.h"

#define WIDTH 960
#define HEIGHT 540

liquid::HTMLDocument load_from_result(network::ResolveResult& result)
{
  liquid::HTMLDocument document;
  if (result.from_file) 
    document.from_file(result.content);
  else
    document.from_string(result.content);

  return document;
}

int main(int argc, char* argv[])
{
  std::string url = "https://example.com/";
  auto result = network::resolve(url);

	liquid::HTMLDocument document = load_from_result(result);

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Raptor", sf::Style::Default);

  window.setTitle(document.title);

  liquid::Viewport viewport(document.body, WIDTH, HEIGHT);

  window.clear(sf::Color::White);
  paint(window, viewport.tree, viewport);
  window.display();

  while (window.isOpen())
  {
    sf::Event event = sf::Event();
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::Resized)
      {
        std::cout << "Width: " << event.size.width << " Height: " << event.size.height << std::endl;

        sf::View resized_view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
        window.setView(resized_view);

        viewport.update_width(event.size.width);
        viewport.update_height(event.size.height);

        window.clear(sf::Color::White);
        paint(window, viewport.tree, viewport);
        window.display();
      }
      else if (event.type == sf::Event::MouseWheelScrolled)
      {
        std::cout << "Mouse wheel scrolled: " << event.mouseWheelScroll.delta << std::endl;
        if (event.mouseWheelScroll.delta > 0)
          viewport.scroll_up();
        else if (event.mouseWheelScroll.delta < 0)
          viewport.scroll_down();

        window.clear(sf::Color::White);
        paint(window, viewport.tree, viewport);
        window.display();
      }
      else if (event.type == sf::Event::KeyPressed and event.key.control and sf::Keyboard::isKeyPressed(sf::Keyboard::R)) 
      {
        liquid::HTMLDocument document = liquid::HTMLDocument();
	      document = load_from_result(result);
        window.setTitle(document.title);

        viewport  = liquid::Viewport(document.body, WIDTH, HEIGHT);

        window.clear(sf::Color::White);
        paint(window, viewport.tree, viewport);
        window.display();
      }
    }
  }
  
  return 0;
}
