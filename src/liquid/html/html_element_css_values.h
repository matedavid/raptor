#pragma once

#include <vector>
#include <string>
#include <utility>

struct HTMLElementCSSValues
{
  // Background
  std::string background_color = "transparent";
  std::string background_image = "none";
  std::pair<std::string, std::string> background_position = {"0%", "0%"};
  std::string background_size; // TODO
  std::string background_repeat = "repeat";
  std::string background_origin = "padding-box";
  std::string background_clip = "border-box";
  std::string background_attachment = "scroll";

  // Margin
  float margin_top    = 0.0;
  float margin_bottom = 0.0;
  float margin_right  = 0.0;
  float margin_left   = 0.0;
};