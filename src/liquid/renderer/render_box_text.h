#pragma once

#include <iostream>

#include <SFML/Graphics/Text.hpp>

#include "liquid/renderer/render_box.h"
#include "liquid/html/text.h"

namespace liquid {

class RenderBoxText : public RenderBox
{
public:
  Text* text;

public:
  RenderBoxType type() const override { return RenderBoxType::Txt; }
  bool is_printable()  const override { return true; }

  RenderBoxText(Text* text, RenderBox* parent);

  // Computes the height of the Text RenderBox
  AppliedDimensions compute_dimensions(float, float) override;

  // Returns the content of the text
  std::string content() const;

  void print(int n_tabs) override;
};

}
