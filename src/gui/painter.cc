#include "painter.h"

sf::VertexArray paint_text_underline(const liquid::RenderBoxText* text, liquid::Color& color)
{
  sf::Color c = sf::Color(color.red, color.green, color.blue, color.alpha*255.f);

  sf::VertexArray underline(sf::Lines, 2);
  underline[0].position = sf::Vector2f(text->get_x(), text->get_y()+text->get_content_height());
  underline[0].color = c;
  underline[1].position = sf::Vector2f(text->get_x()+text->get_content_width(), text->get_y()+text->get_content_height());
  underline[1].color = c;

  return underline;
}

sf::VertexArray paint_text_overline(const liquid::RenderBoxText* text, liquid::Color& color)
{
  sf::Color c = sf::Color(color.red, color.green, color.blue, color.alpha*255.f);

  sf::VertexArray overline(sf::Lines, 2);
  overline[0].position = sf::Vector2f(text->get_x(), text->get_y());
  overline[0].color = c;
  overline[1].position = sf::Vector2f(text->get_x()+text->get_content_width(), text->get_y());
  overline[1].color = c;

  return overline;
}

sf::VertexArray paint_text_line_through(const liquid::RenderBoxText* text, liquid::Color& color)
{
  float middle = (text->get_y() + text->get_y()+text->get_content_height())/2.f;
  sf::Color c = sf::Color(color.red, color.green, color.blue, color.alpha*255.f);

  sf::VertexArray line_through(sf::Lines, 2);
  line_through[0].position = sf::Vector2f(text->get_x(), middle);
  line_through[0].color = c;
  line_through[1].position = sf::Vector2f(text->get_x()+text->get_content_width(), middle);
  line_through[1].color = c;

  return line_through;
}

void paint_text(sf::RenderWindow& window, liquid::RenderBoxText* render_box_text)
{
  // Apply Text css elements
  auto text_element = dynamic_cast<liquid::Text*>(render_box_text->node);
  if (text_element == nullptr)
    return;

  float font_size = render_box_text->get_font_size();
  std::string content = render_box_text->get_content();

  sf::Font font;
  if (not font.loadFromFile("../src/gui/Fonts/LiberationSans-Regular.ttf"))
    return;

  sf::Text text;
  text.setFont(font);
  text.setString(content);
  text.setPosition(render_box_text->get_x(), render_box_text->get_y());

  // font-size
  text.setCharacterSize((unsigned int)font_size);
  
  // color
  sf::Color text_color = sf::Color(
    text_element->style.color.red,
    text_element->style.color.green,
    text_element->style.color.blue,
    text_element->style.color.alpha*255
  );
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

void paint_image(sf::RenderWindow& window, liquid::RenderBoxImage* render_box_image)
{
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
    scale_factor.x = render_box_image->img_width / float(image_size.x);
  if (render_box_image->img_height == -1)
    scale_factor.y = 1.;
  else
    scale_factor.y = render_box_image->img_height / float(image_size.y);

  image_sprite.setScale(scale_factor);
  window.draw(image_sprite);
}

void paint(sf::RenderWindow& window, liquid::RenderBox* render_tree)
{
  if (render_tree->is_printable())
  {
    if (render_tree->type() == liquid::RenderBoxType::Txt)
    {
      auto render_box_text = dynamic_cast<liquid::RenderBoxText*>(render_tree);
      paint_text(window, render_box_text);
    }
    else if (render_tree->type() == liquid::RenderBoxType::Img)
    {
      auto render_box_image = dynamic_cast<liquid::RenderBoxImage*>(render_tree);
      paint_image(window, render_box_image);
    }
    return;
  }

  if (render_tree->node->style.background_color.alpha != 0.f)
  {
    liquid::Color c = render_tree->node->style.background_color;

    sf::RectangleShape background;
    background.setPosition(render_tree->get_border_x(), render_tree->get_border_y());
    background.setSize(sf::Vector2f(render_tree->get_width(), render_tree->get_height()));
    background.setFillColor(sf::Color(c.red, c.green, c.blue, c.alpha*255.f));

    window.draw(background);
  }

  for (liquid::RenderBox* child : render_tree->get_children())
  {
    paint(window, child);
  }
}