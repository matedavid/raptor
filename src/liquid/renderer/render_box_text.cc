#include "render_box_text.h"

namespace liquid {

RenderBoxText::RenderBoxText()
{
}

RenderBoxText::RenderBoxText(Text* text_element, RenderBox* parent)
{
  this->parent = parent;
  node = text_element;
  content = text_element->content();
}

void RenderBoxText::layout(float container_width)
{
  auto [xref, yref] = compute_xy_reference();
  x = xref;
  y = yref;


  // TODO: Find better way to do this?
  sf::Font font;
  if (not font.loadFromFile("/home/david/workspace/raptor/src/gui/Fonts/Arial-Font/arial_1.ttf"))
  {
    std::cout << "Error loading font" << std::endl;
  }

  sf::Text t(content, font, node->style.font_size);

  // Compute width
  content_width = box_width = t.getLocalBounds().width;

  // Compute height
  content_height = box_height = node->style.font_size * 1.3;
}

std::string RenderBoxText::split_content(uint container_width)
{
  float character_width = float(content_width)/content.size();
  int n = container_width/character_width;

  while (content[n] != ' ')
    --n;

  std::string actual_content = content.substr(0, n);
  std::string split_content = content.substr(n+1, content.size());

  set_content(actual_content);
  return split_content;
}

std::string RenderBoxText::get_content() const 
{
  return content;
}

void RenderBoxText::set_content(const std::string& new_content)
{
  content = new_content;
  //text_width = content.length() * character_width;

  sf::Font font;
  if (not font.loadFromFile("/home/david/workspace/raptor/src/gui/Fonts/Arial-Font/arial_1.ttf"))
  {
    std::cout << "Error loading font" << std::endl;
  }

  sf::Text t(content, font, node->style.font_size);
  //text_width = t.getLocalBounds().width;
  content_width = box_width = t.getLocalBounds().width;
}

float RenderBoxText::get_font_size() const
{
  return node->style.font_size;
}

}