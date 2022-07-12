#pragma once

#include <iostream>

#include <SFML/Graphics/Text.hpp>

#include "liquid/renderer/render_box.h"
#include "liquid/html/text.h"

namespace liquid {

class RenderBoxText : public RenderBox
{
private:
  Text* text;

public:
  RenderBoxText(Text* text, RenderBox* parent);

  // Computes the height of the Text RenderBox
  Dimensions compute_dimensions(float) override;

  // Returns the content of the text
  std::string content() const;

  void print(int n_tabs) override;
};

}
