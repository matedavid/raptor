#pragma once

#include <iostream>

#include "render_box.h"
#include "render_box_text.h"
#include "render_box_img.h"
#include "render_box_marker.h"

#include "liquid/html/html_element.h"
#include "liquid/html/html_image_element.h"
#include "liquid/html/text.h"

namespace liquid {

struct RendererConfig
{
  RenderBox* list_renderbox = nullptr; // The <ol> or <ul> RenderBox instance or nullptr
  int counter = 0; // Counter for counted list markers (e.g. <ol>)
};

RenderBox* generate_render_tree(HTMLElement* element, RenderBox* parent, float width, RendererConfig& cfg);

}