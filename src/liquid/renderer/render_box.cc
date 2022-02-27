#include "render_box.h"

namespace liquid 
{

RenderBox::RenderBox()
{

}

RenderBox::RenderBox(const HTMLElement* element)
{
  node = (HTMLElement*)element;
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

  /*
  // 2. For all the node's children
  //    2.1 Place the child renderer (set x and y)
  //    2.2 Call children layout function
  float accumulated_height = 0.;
  for (HTMLElement* child : node->child_elements())
  {
    if (child->type() == HTMLElementType::TextType)
    {
      Text* text_element = dynamic_cast<Text*>(child);
      if (text_element == nullptr)
        continue;

      RenderBoxText* text_render = new RenderBoxText(text_element);
      accumulated_height += text_render->get_height();

      children.push_back(text_render);
    }
    else
    {
      RenderBox* child_render_box = new RenderBox(child, width, _height);
      if (child_render_box->type != RenderBoxType::Inline)
        accumulated_height += child_render_box->get_height();

      children.push_back(child_render_box);
    }
  }

  // 3. Use the accumulated height value form children to compute height value
  height = accumulated_height;
  */
}

void RenderBox::print()
{
  std::cout << "(" << node->element_value << ") RenderBox: (" << x << "," << y << ") width=" << width << " height="  << height << std::endl;
  for (auto child : children)
  {
    child->print();
  }
}

}