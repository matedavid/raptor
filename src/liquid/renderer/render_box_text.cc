#include "render_box_text.h"

#include "cmath"

namespace liquid {

float RenderBoxText::get_text_width(const std::string& content, float font_size)
{
  // TODO: Find better way to do this?
  sf::Font font;
  if (not font.loadFromFile("/home/david/workspace/raptor/src/gui/Fonts/Arial-Font/arial_1.ttf"))
    std::cout << "Error loading font" << std::endl;

  sf::Text t(content, font, (uint)font_size);
  return t.getLocalBounds().width;
}

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

  // Compute width
  //content_width = box_width = t.getLocalBounds().width;
  content_width = get_text_width(content, node->style.font_size);

  // Compute height (approximation)
  content_height = node->style.font_size * 1.25f;
}

std::string RenderBoxText::split_content(float container_width)
{
  float character_width = content_width/float(content.size());
  int n = container_width/character_width;

  while (n >= 0 and (content[n] != ' ' or get_text_width(content.substr(0, n), node->style.font_size) >= container_width))
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

  content_width = get_text_width(content, node->style.font_size);
}

float RenderBoxText::get_font_size() const
{
  return node->style.font_size;
}

}