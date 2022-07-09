#include "render_box_text.h"

namespace liquid {

RenderBoxText::RenderBoxText(Text* text, RenderBox* parent)
  : text(text), RenderBox((HTMLElement*)text, parent)
{
}

Dimensions RenderBoxText::compute_dimensions(float)
{
  sf::Font font;
  if (not font.loadFromFile("../src/gui/Fonts/LiberationSans-Regular.ttf"))
  {
    std::cout << "Error loading font: compute_height" << std::endl;
    exit(0);
  }

  sf::Text t(content(), font, text->style.font_size);
  content_width = width = content_width = t.getGlobalBounds().width;
  content_height = height = margin_height = t.getGlobalBounds().height;

  return {.width=width, .height=height};
}

std::string RenderBoxText::content() const
{
  return text->content();
}

void RenderBoxText::print(int n_tabs)
{
  for (int i = 0; i < n_tabs; ++i)
    std::cout << "  ";

  printf("[Text]: %s\n", content().c_str());
}

}
