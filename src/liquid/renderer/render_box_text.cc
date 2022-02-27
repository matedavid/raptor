#include "render_box_text.h"

namespace liquid {

RenderBoxText::RenderBoxText()
{
}

RenderBoxText::RenderBoxText(Text* text_element)
{
  node = text_element;
  content = text_element->content();

  // Compute height
  height = text_element->style.font_size * 1.2;
}

}