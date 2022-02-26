#include "render_box.h"

namespace liquid 
{

RenderBox::RenderBox()
{

}

RenderBox::RenderBox(const HTMLElement* element)
{
  layout(element);
}

void RenderBox::layout(const HTMLElement* element)
{
  // Things to compute + things to take into account
  //  1. values for margin, padding and border edges - css "margin", "pading", "border-width"
  //  2. width, height - css "width" & "height"
  //  3. x,y positioning - css "position"

  // General workflow:
  // 1. Determine width
  // 2. For all the element's children
  //    2.1 Place the child renderer (set x and y)
  //    2.2 Call children layout function
  // 3. Use the accumulated height value form children to compute height value

}

}