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
  RenderBoxType type() const override { return RenderBoxType::Txt; }

  float text_width;
  float character_width;

public:
  RenderBoxText();
  RenderBoxText(Text* text_element, RenderBox* parent);

  std::string split_content();

  std::string get_content() const;
  void set_content(const std::string& new_content);
  float get_font_size() const;
};

}