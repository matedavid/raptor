#pragma once

#include "render_box.h"

#include "liquid/html/html_element.h"
#include "liquid/html/html_image_element.h"

namespace liquid {

class RenderBoxImage : public RenderBox
{
public:
  float img_width = -1.;
  float img_height = -1.;

  bool is_printable() const override { return true; }
  RenderBoxType type() const override { return RenderBoxType::Img; }

public:
  RenderBoxImage();
  RenderBoxImage(HTMLImageElement* img_element, RenderBox* parent);

  std::string get_src() const;
  std::string get_alt() const;
};

}