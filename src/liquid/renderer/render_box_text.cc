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

}