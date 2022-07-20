#pragma once

#include "liquid/html/html_element.h"
#include "liquid/renderer/render_box.h"
#include "liquid/renderer/render_box_text.h"
#include "liquid/renderer/render_box_image.h"

namespace liquid {

class Viewport
{
public:
  RenderBox* render_tree;

private:
  static RenderBox* construct_render_tree(HTMLElement* element, RenderBox* parent);

public:
  Viewport(HTMLElement* body, float width, float height);
};

}
