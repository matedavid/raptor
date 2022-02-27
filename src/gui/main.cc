#include <iostream>

#include <SFML/Graphics.hpp>

#include "liquid/html/html_document.h"
#include "liquid/renderer/render_tree.h"


void paint(sf::RenderWindow& window, liquid::RenderBox* render_tree)
{
  if (render_tree->is_printable())
  {
    liquid::RenderBoxText* render_box_text = dynamic_cast<liquid::RenderBoxText*>(render_tree);
    if (render_box_text == nullptr)
      return;

    float font_size = render_box_text->get_font_size();
    std::string content = render_box_text->get_content();

    sf::Font font;
    if (not font.loadFromFile("../src/gui/Roboto/Roboto-Regular.ttf"))
      return;

    sf::Text text;
    text.setString(content);
    text.setFont(font);
    text.setCharacterSize(font_size);
    text.setPosition(render_box_text->get_x(), render_box_text->get_y());
    text.setFillColor(sf::Color::Black);

    window.draw(text);
  }

  for (liquid::RenderBox* child : render_tree->get_children())
  {
    paint(window, child);
  }
}

int main(int argc, char* argv[])
{
  sf::RenderWindow window(sf::VideoMode(1024, 512), "Raptor", sf::Style::Default);


	std::filesystem::path file_path = "/home/david/workspace/raptor/examples/index.html";
	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);

	liquid::RenderBox* render_tree = liquid::generate_render_tree(document.body, nullptr, 1024);

  window.clear(sf::Color::White);
  paint(window, render_tree);
  window.display();

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      else if (event.type == sf::Event::Resized)
      {
        std::cout << "Width: " << event.size.width << " Height: " << event.size.height << std::endl;
        render_tree = liquid::generate_render_tree(document.body, nullptr, event.size.width);
        window.clear(sf::Color::White);
        paint(window, render_tree);
        window.display();
      }
    }

  }
  
  return 0;
}
