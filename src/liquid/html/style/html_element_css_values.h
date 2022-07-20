#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <limits>

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

// TODO: This is bad idea, should not do this
// Temporal solution until I come up with better implementation on how to
// represent css values
const float AUTO = -std::numeric_limits<float>::max();

const float BORDER_WIDTH_THIN   = 1.f;
const float BORDER_WIDTH_MEDIUM = 3.f;
const float BORDER_WIDTH_THICK  = 5.f;

struct HTMLElementCSSValues
{
  Color color = Color{0, 0, 0}; // black
  float width  = AUTO; // auto
  float height = AUTO; // auto

  std::string display = "inline";
  std::string position = "static";

  // Top,bottom,left,right
  float top    = AUTO;
  float bottom = AUTO; 
  float left   = AUTO;
  float right  = AUTO;

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
  float margin_top    = 0.f;
  float margin_bottom = 0.f;
  float margin_right  = 0.f;
  float margin_left   = 0.f;

  // Padding
  float padding_top    = 0.f;
  float padding_bottom = 0.f;
  float padding_right  = 0.f;
  float padding_left   = 0.f;

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
