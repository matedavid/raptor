#pragma once

#include "render_box.h"

#include "liquid/html/html_element.h"
#include "liquid/html/text.h"

#include <SFML/Graphics/Text.hpp>

namespace liquid {

class RenderBoxText : public RenderBox
{
private:
  std::string content;

public:
  bool is_printable() const override { return true; }
  RenderBoxType type() const override { return RenderBoxType::Txt; }

private:
  static float get_text_width(const std::string& content, float font_size);

public:
  RenderBoxText();
  RenderBoxText(Text* text_element, RenderBox* parent);

  void layout(float container_width) override;

  std::string split_content(float container_width);

  std::string get_content() const;
  void set_content(const std::string& new_content);
  float get_font_size() const;
};

}