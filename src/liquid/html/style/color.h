#pragma once

#include <iostream>
#include <regex>

namespace liquid {

// Color value documentation: https://developer.mozilla.org/en-US/docs/Web/CSS/color_value
struct Color
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  float alpha = 1.;
};

// Converts a textual color (red, blue, green...) to liquid::Color object
Color color_from_string(const std::string& color_name);

// Converts a CSS color value to liquid::Color object
Color parse_color(const std::string& value);

}
