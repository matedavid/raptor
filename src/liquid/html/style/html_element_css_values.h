#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>

struct HTMLElementCSSValues
{
  std::string color = "black";

  // Font
  std::string font_style = "normal";
  std::string font_variant = "normal";
  std::string font_weight = "normal";
  float font_size = 16.0;
  std::string font_family = "Arial";

  // Background
  std::string background_color = "transparent";
  std::string background_image = "none";
  std::pair<std::string, std::string> background_position = {"0.0", "0.0"};
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

  // Padding
  float padding_top    = 0.0;
  float padding_bottom = 0.0;
  float padding_right  = 0.0;
  float padding_left   = 0.0;

  // Text decoration
  std::vector<std::string> text_decoration_line = {"none"};
  std::string text_decoration_color = "black";
  std::string text_decoration_style = "solid";

  // Border (vector represents = {top, right, bottom, left})
  std::vector<std::string> border_width = {"medium", "medium", "medium", "medium"};
  std::vector<std::string> border_style = {"none", "none", "none", "none"};
  std::vector<std::string> border_color = {"black"};
};
