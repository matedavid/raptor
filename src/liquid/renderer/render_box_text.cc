#include "render_box_text.h"

namespace liquid {

RenderBoxText::RenderBoxText(Text* text, RenderBox* parent)
  : text(text), RenderBox((HTMLElement*)text, parent)
{
  display = RenderBoxDisplay::Inline;
}

AppliedDimensions RenderBoxText::compute_dimensions(float, float)
{
  sf::Font font;
  if (not font.loadFromFile("/home/david/workspace/raptor/src/gui/Fonts/LiberationSans-Regular.ttf"))
  {
    std::cout << "Error loading font: compute_height" << std::endl;
    exit(0);
  }

  sf::Text t(content(), font, text->style.font_size);
  content_width = width = t.getGlobalBounds().width;
  content_height = height = t.getGlobalBounds().height;

  return {.width=true, .height=true};
}

std::string RenderBoxText::content() const
{
  return text->content();
}

void RenderBoxText::print(int n_tabs)
{
  for (int i = 0; i < n_tabs; ++i)
    std::cout << "  ";

  printf("[Text pos=(%.1f, %.1f)]: %s\n", x, y, content().c_str());
}

}
