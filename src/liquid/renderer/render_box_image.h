#pragma once

#include <iostream>

#include "liquid/renderer/render_box.h"
#include "liquid/html/html_image_element.h"

#include "SFML/Graphics/Image.hpp"

namespace liquid {

class RenderBoxImage : public RenderBox
{
public:
  HTMLImageElement* image;

public:
  RenderBoxType type() const override { return RenderBoxType::Img; }
  bool is_printable()  const override { return true; }

  RenderBoxImage(HTMLImageElement* image, RenderBox* parent);

  // Computes the height of the Text RenderBox
  AppliedDimensions compute_dimensions(float) override;

  float get_width()  const { return width;  }
  float get_height() const { return height; }

  void print(int n_tabs) override;
};

}
