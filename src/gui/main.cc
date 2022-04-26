#include <iostream>

#include <SFML/Graphics.hpp>

#include "liquid/html/html_document.h"
#include "liquid/renderer/viewport.h"
#include "painter.h"

#define WIDTH 960
#define HEIGHT 540

int main(int argc, char* argv[])
{
	std::filesystem::path file_path;
  if (argc > 1)
    file_path = argv[1];
  else
    file_path = "/home/david/workspace/raptor/examples/index.html";

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Raptor", sf::Style::Default);

	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);
  window.setTitle(document.title);

  liquid::Viewport viewport(document.body, WIDTH, HEIGHT);

	//liquid::RenderBox* render_tree = liquid::generate_render_tree(document.body, nullptr, 960);
  //render_tree->print(0);

  window.clear(sf::Color::White);
  //paint(window, render_tree);
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

        //render_tree = liquid::generate_render_tree(document.body, nullptr, event.size.width);
        window.clear(sf::Color::White);
        //paint(window, render_tree);
        window.display();
      }
      else if (event.type == sf::Event::MouseWheelScrolled)
      {
        std::cout << "Mouse wheel scrolled: " << event.mouseWheelScroll.delta << std::endl;
      }
    }
  }
  
  return 0;
}
