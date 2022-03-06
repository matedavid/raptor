#pragma once

#include <iostream>

namespace liquid {

// Color value documentation: https://developer.mozilla.org/en-US/docs/Web/CSS/color_value
struct Color
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

Color color_from_string(const std::string& color_name);

}
