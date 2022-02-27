#pragma once

#include <iostream>

#include "render_box.h"
#include "render_box_text.h"

namespace liquid {

RenderBox* generate_render_tree(HTMLElement* element, RenderBox* parent, float _width);

}