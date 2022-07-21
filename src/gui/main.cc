#include <iostream>
#include <SFML/Graphics.hpp>

#include "liquid/html/html_document.h"
#include "liquid/renderer/viewport.h"
#include "painter.h"

#include "network/network.h"

#define WIDTH 960.f
#define HEIGHT 540.f

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
  std::string url = "file:///home/david/workspace/raptor/examples/index.html";
  auto result = network::resolve(url);

	liquid::HTMLDocument document = load_from_result(result);

  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Raptor", sf::Style::Default);
  window.setTitle(document.title);

  liquid::Viewport viewport(document.body, WIDTH, HEIGHT);
  viewport.render_tree->print(0);

  window.clear(sf::Color::White);
  paint(window, viewport.render_tree);
  window.display();

  while (window.isOpen())
  {
    sf::Event event = sf::Event();
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
  }
  
  return 0;
}
