#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "color.h"

namespace liquid {

template<typename T> 
struct EdgeValues
{
  T top;
  T right;
  T bottom;
  T left;
};

const float BORDER_WIDTH_THIN   = 1.f;
const float BORDER_WIDTH_MEDIUM = 3.f;
const float BORDER_WIDTH_THICK  = 5.f;

struct HTMLElementCSSValues
{
  Color color = Color{0, 0, 0}; // black
  float width  = -1;  // -1 = auto
  float height = -1;  // -1 = auto

  std::string display = "inline";
  std::string position = "static";

  // Top,bottom,left,right
  float top    = 0;
  float bottom = 0;
  float left   = 0;
  float right  = 0;

  // Font
  std::string font_style = "normal";
  std::string font_variant = "normal";
  std::string font_weight = "normal";
  float font_size = 16.0;
  std::string font_family = "Arial";

  // Background
  Color background_color = Color{255, 255, 255, 0.0}; // Default color: transparent (alpha = 0.0)
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
  Color text_decoration_color = Color{0, 0, 0}; // Default color: black
  std::string text_decoration_style = "solid";

  // Border (vector represents = {top, right, bottom, left})
  EdgeValues<float> border_width = {BORDER_WIDTH_MEDIUM, BORDER_WIDTH_MEDIUM, BORDER_WIDTH_MEDIUM, BORDER_WIDTH_MEDIUM};
  EdgeValues<std::string> border_style = {"none", "none", "none", "none"};
  EdgeValues<Color> border_color = { Color{0,0,0}, Color{0,0,0}, Color{0,0,0}, Color{0,0,0} }; // black
};

}
