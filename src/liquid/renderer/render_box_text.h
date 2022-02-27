#pragma once

#include "render_box.h"

#include "liquid/html/html_element.h"
#include "liquid/html/text.h"

namespace liquid {

class RenderBoxText : public RenderBox
{
private:
  std::string content;

public:
  bool is_printable() const override { return true; }

public:
  RenderBoxText();
  RenderBoxText(Text* text_element, RenderBox* parent);

  std::string get_content() const;
  float get_font_size() const;
};

}