#include "render_box.h"

namespace liquid 
{

RenderBox::RenderBox()
{
}

RenderBox::RenderBox(HTMLElement* element, RenderBox* parent)
  : node(element), parent(parent)
{
}

void RenderBox::layout(uint _width)
{
  // Things to compute + things to take into account
  //  1. values for margin, padding and border edges - css "margin", "pading", "border-width"
  //  2. width, height - css "width" & "height"
  //  3. x,y positioning - css "position"

  // General workflow:
  // 1. Determine width and (x,y)
  width = _width - (node->style.margin_left+node->style.margin_right + node->style.padding_left+node->style.padding_right);

  float xref, yref;
  if (parent == nullptr)
  {
    xref = yref = 0;
  }
  else
  {
    xref = parent->get_x();
    yref = parent->get_y();
  }

  x = xref + node->style.margin_left + node->style.padding_left;
  y = yref + node->style.margin_top + node->style.padding_top;
}

void RenderBox::print(int number_tabs)
{
  for (int i = 0; i < number_tabs; ++i)
  {
    std::cout << "  ";
  }

  std::cout << "(" << node->element_value << ") RenderBox: (" << x << "," << y << ") width=" << width << " height="  << height << std::endl;
  for (auto child : children)
  {
    child->print(number_tabs+1);
  }
}

}