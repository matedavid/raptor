#include "painter.h"

void paint_text(sf::RenderWindow& window, liquid::RenderBoxText* render_box_text)
{
  // Apply Text css elements
  auto text_element = dynamic_cast<liquid::Text*>(render_box_text->text);

  float font_size = render_box_text->text->style.font_size;
  std::string content = render_box_text->content();

  sf::Font font;
  if (not font.loadFromFile("/home/david/workspace/raptor/src/gui/Fonts/LiberationSans-Regular.ttf"))
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

  window.draw(text);
}

void paint_image(sf::RenderWindow& window, liquid::RenderBoxImage* render_box_image)
{
  std::string src = render_box_image->image->src();

  sf::Image image;
  if (not image.loadFromFile(src))
  {
    std::cout << "Error loading image, showing alt: " << render_box_image->image->alt() << std::endl;
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
  scale_factor.x = render_box_image->get_width() / float(image_size.x);
  scale_factor.y = render_box_image->get_height() / float(image_size.y);

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
   
  for (liquid::RenderBox* child : render_tree->get_children())
  {
    paint(window, child);
  }
}
