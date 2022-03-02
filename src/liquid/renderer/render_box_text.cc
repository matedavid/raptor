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

  // Compute x, y position
  compute_xy_position();

  // Compute width
  width = parent->get_width();

  character_width = text_element->style.font_size * 0.793333;
  text_width = content.length() * character_width;

  // Compute height
  height = text_element->style.font_size * 1.2;
}

std::string RenderBoxText::split_content()
{
  float number_characters = width/character_width;
  while (content[number_characters] != ' ')
    --number_characters;

  std::string actual_content = content.substr(0, number_characters);
  std::string split_content = content.substr(number_characters+1, content.size());

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
  text_width = content.length() * character_width;
}

float RenderBoxText::get_font_size() const
{
  return node->style.font_size;
}

}