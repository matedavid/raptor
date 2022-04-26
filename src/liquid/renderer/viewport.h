#pragma once

#include "liquid/html/html_element.h"
#include "liquid/renderer/render_box.h"
#include "liquid/renderer/render_tree.h"

namespace liquid {

class Viewport 
{
private:
  RenderBox* tree;

  float width, height;
  float x, y;
  float lowest_y;

  float scroll_factor = 2.f;

private:
  static void compute_lowest_y(RenderBox* tree, float& min);

public:
  Viewport(HTMLElement* element, float width, float height);

  void update_width(float new_width);
  void update_height(float new_height);
  
  void scroll_up();
  void scroll_down();
};

}