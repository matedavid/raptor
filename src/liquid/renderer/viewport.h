#pragma once

#include "liquid/html/html_element.h"
#include "liquid/renderer/render_box.h"
#include "liquid/renderer/render_box_text.h"

namespace liquid {

class Viewport
{
private:
  static RenderBox* construct_render_tree(HTMLElement* element, RenderBox* parent);
  static void compute_height(RenderBox* render_box);

public:
  Viewport(HTMLElement* body);
};

}
