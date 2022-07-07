#include "render_box_text.h"

namespace liquid {

RenderBoxText::RenderBoxText(Text* text, RenderBox* parent)
  : text(text), RenderBox((HTMLElement*)text, parent)
{
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
