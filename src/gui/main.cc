#include <iostream>

#include <SFML/Graphics.hpp>

#include "liquid/html/html_document.h"
#include "liquid/renderer/render_tree.h"


void paint(sf::RenderWindow& window, liquid::RenderBox* render_tree)
{
  if (render_tree->is_printable())
  {
    if (render_tree->type() == liquid::RenderBoxType::Txt)
    {
      liquid::RenderBoxText* render_box_text = dynamic_cast<liquid::RenderBoxText*>(render_tree);
      if (render_box_text == nullptr)
        return;

      float font_size = render_box_text->get_font_size();
      std::string content = render_box_text->get_content();

      sf::Font font;
      if (not font.loadFromFile("/home/david/workspace/raptor/src/gui/Fonts/Arial-Font/arial_1.ttf"))
        return;

      sf::Text text;
      text.setString(content);
      text.setFont(font);
      text.setCharacterSize(font_size);
      text.setPosition(render_box_text->get_x(), render_box_text->get_y());
      text.setFillColor(sf::Color::Black);

      window.draw(text);
    }
    else if (render_tree->type() == liquid::RenderBoxType::Img)
    {
      liquid::RenderBoxImage* render_box_image = dynamic_cast<liquid::RenderBoxImage*>(render_tree);
      if (render_box_image == nullptr)
        return;

      std::string src = render_box_image->get_src();

      sf::Image image;
      if (not image.loadFromFile(src))
      {
        std::cout << "Error loading image, showing alt: " << render_box_image->get_alt() << std::endl;
        return;
      }

      sf::Vector2u image_size = image.getSize();

      sf::Texture image_texture;
      if (not image_texture.loadFromImage(image))
      {
        std::cout << "Error loading texture from Image" << std::endl;
        return;
      }

      sf::Sprite image_sprite;
      image_sprite.setTexture(image_texture, true);
      image_sprite.setPosition(render_box_image->get_x(), render_box_image->get_y());

      // Apply width and height to sprite
      sf::Vector2f scale_factor;
      if (render_box_image->img_width == -1)
        scale_factor.x = 1.;
      else 
        scale_factor.x = render_box_image->img_width / image_size.x;
      if (render_box_image->img_height == -1)
        scale_factor.y = 1.;
      else
        scale_factor.y = render_box_image->img_height / image_size.y;

      image_sprite.setScale(scale_factor);
      window.draw(image_sprite);
    }
  }

  // Print margin, padding lines
  sf::VertexArray margin_top(sf::Lines, 2);
  margin_top[0].position = sf::Vector2f(render_tree->get_x(), render_tree->margin.top);
  margin_top[0].color = sf::Color::Red;
  margin_top[1].position = sf::Vector2f(render_tree->get_x()+render_tree->get_width(), render_tree->margin.top);
  margin_top[1].color = sf::Color::Red;

  sf::VertexArray margin_right(sf::Lines, 2);
  margin_right[0].position = sf::Vector2f(render_tree->margin.right, render_tree->get_y()-render_tree->node->style.margin_top);
  margin_right[0].color = sf::Color::Red;
  margin_right[1].position = sf::Vector2f(render_tree->margin.right, render_tree->get_y()+render_tree->get_height()+render_tree->node->style.margin_bottom);
  margin_right[1].color = sf::Color::Red;

  sf::VertexArray margin_bottom(sf::Lines, 2);
  margin_bottom[0].position = sf::Vector2f(render_tree->get_x(), render_tree->margin.bottom);
  margin_bottom[0].color = sf::Color::Red;
  margin_bottom[1].position = sf::Vector2f(render_tree->get_x()+render_tree->get_width(), render_tree->margin.bottom);
  margin_bottom[1].color = sf::Color::Red;

  sf::VertexArray margin_left(sf::Lines, 2);
  margin_left[0].position = sf::Vector2f(render_tree->margin.left, render_tree->get_y()-render_tree->node->style.margin_top);
  margin_left[0].color = sf::Color::Red;
  margin_left[1].position = sf::Vector2f(render_tree->margin.left, render_tree->get_y()+render_tree->get_height()+render_tree->node->style.margin_bottom);
  margin_left[1].color = sf::Color::Red;

  //window.draw(margin_top);
  //window.draw(margin_right);
  //window.draw(margin_bottom);
  //window.draw(margin_left);

  for (liquid::RenderBox* child : render_tree->get_children())
  {
    paint(window, child);
  }
}

int main(int argc, char* argv[])
{
	std::filesystem::path file_path;
  if (argc > 1)
    file_path = argv[1];
  else
    file_path = "/home/david/workspace/raptor/examples/index.html";

  sf::RenderWindow window(sf::VideoMode(1024, 512), "Raptor", sf::Style::Default);

	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);

  window.setTitle(document.title);

	liquid::RenderBox* render_tree = liquid::generate_render_tree(document.body, nullptr, 1024);
  render_tree->print(0);

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

        sf::View new_view = sf::View(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
        window.setView(new_view);

        render_tree = liquid::generate_render_tree(document.body, nullptr, event.size.width);
        window.clear(sf::Color::White);
        paint(window, render_tree);
        window.display();
      }
    }

  }
  
  return 0;
}
