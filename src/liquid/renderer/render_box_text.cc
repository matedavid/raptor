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
  x = parent->get_x();
  y = parent->get_y();

  // Compute width
  width = parent->get_width();

  // Compute height
  height = text_element->style.font_size * 1.2;
}

std::string RenderBoxText::get_content() const 
{
  Text* text_node = dynamic_cast<Text*>(node);
  if (text_node == nullptr)
    return "";

  return text_node->content();
}

float RenderBoxText::get_font_size() const
{
  return node->style.font_size;
}

}