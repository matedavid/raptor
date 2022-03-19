#include <iostream>

#include <SFML/Graphics.hpp>

#include "liquid/html/html_document.h"
#include "liquid/html/text.h"
#include "liquid/renderer/render_tree.h"

sf::VertexArray paint_text_underline(const liquid::RenderBoxText* text, liquid::Color& color)
{
  sf::Color c = sf::Color(color.red, color.green, color.blue);

  sf::VertexArray underline(sf::Lines, 2);
  underline[0].position = sf::Vector2f(text->get_x(), text->get_y()+text->get_height()+1);
  underline[0].color = c;
  underline[1].position = sf::Vector2f(text->get_x()+text->get_width(), text->get_y()+text->get_height()+1);
  underline[1].color = c;

  return underline;
}

sf::VertexArray paint_text_overline(const liquid::RenderBoxText* text, liquid::Color& color)
{
  sf::Color c = sf::Color(color.red, color.green, color.blue);

  sf::VertexArray overline(sf::Lines, 2);
  overline[0].position = sf::Vector2f(text->get_x(), text->get_y());
  overline[0].color = c;
  overline[1].position = sf::Vector2f(text->get_x()+text->get_width(), text->get_y());
  overline[1].color = c;

  return overline;
}

sf::VertexArray paint_text_line_through(const liquid::RenderBoxText* text, liquid::Color& color)
{
  float middle = (text->get_y() + text->get_y()+text->get_height())/2.;
  sf::Color c = sf::Color(color.red, color.green, color.blue);

  sf::VertexArray line_through(sf::Lines, 2);
  line_through[0].position = sf::Vector2f(text->get_x(), middle);
  line_through[0].color = c;
  line_through[1].position = sf::Vector2f(text->get_x()+text->get_width(), middle);
  line_through[1].color = c;

  return line_through;
}

void paint(sf::RenderWindow& window, liquid::RenderBox* render_tree)
{
  if (render_tree->is_printable())
  {
    if (render_tree->type() == liquid::RenderBoxType::Txt)
    {
      liquid::RenderBoxText* render_box_text = dynamic_cast<liquid::RenderBoxText*>(render_tree);
      if (render_box_text == nullptr)
        return;

      // Apply Text css elements
      liquid::Text* text_element = dynamic_cast<liquid::Text*>(render_box_text->node);
      if (text_element == nullptr)
        return;

      float font_size = render_box_text->get_font_size();
      std::string content = render_box_text->get_content();

      sf::Font font;
      if (not font.loadFromFile("/home/david/workspace/raptor/src/gui/Fonts/Arial-Font/arial_1.ttf"))
        return;

      sf::Text text;
      text.setString(content);
      text.setFont(font);
      text.setPosition(render_box_text->get_x(), render_box_text->get_y());

      // font-size
      text.setCharacterSize(font_size);
      
      // color
      sf::Color text_color = sf::Color(text_element->style.color.red, text_element->style.color.green, text_element->style.color.blue, text_element->style.color.alpha*255);
      text.setFillColor(text_color);

      // font-style
      if (text_element->style.font_style == "italic" or text_element->style.font_style == "oblique")
        text.setStyle(text.getStyle() | sf::Text::Italic);

      // font-weight
      if (text_element->style.font_weight == "bold")
        text.setStyle(text.getStyle() | sf::Text::Bold);

      // text-decoration
      for (std::string& text_decoration_line : text_element->style.text_decoration_line)
      {
        if (text_decoration_line == "none")
          break;
        
        if (text_decoration_line == "underline")
        {
          sf::VertexArray underline = paint_text_underline(render_box_text, text_element->style.text_decoration_color);
          window.draw(underline);
        }
        else if (text_decoration_line == "overline")
        {
          sf::VertexArray overline = paint_text_overline(render_box_text, text_element->style.text_decoration_color);
          window.draw(overline);
        }
        else if (text_decoration_line == "line-through")
        {
          sf::VertexArray line_through = paint_text_line_through(render_box_text, text_element->style.text_decoration_color);
          window.draw(line_through);
        }
      }

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
    return;
  }

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
