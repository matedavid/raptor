#pragma once

#include <gtkmm.h>

#include "liquid/html/html_element.h"

namespace liquid {

// Layout that "emulates" the Box Layout model in CSS
// Used to represent rendered elements in DOM
struct RenderBox
{
  Gtk::Box* outer_box;
  Gtk::Box* inner_box;
  std::string element_value;
};

struct RenderConfig 
{
  // Text
  float font_size = 16.0;                                   // Default font size: 16
  Pango::Weight font_weight = Pango::Weight::WEIGHT_NORMAL; // Default font weight: Normal

  Pango::Underline text_underline = Pango::Underline::UNDERLINE_NONE; // Default: No text-decoration (no underline neither overline)
  Pango::Overline text_overline   = Pango::Overline::OVERLINE_NONE;

  // Lists
  bool list;                  // If element is under a list
  unsigned int list_type;     // ul: 0 - ol: 1
  int unordered_list_marker;  // TODO: Not implemented, should decide how to represent markers
};

RenderBox* new_render_box(const std::string& element_value, Gtk::Orientation orientation);

}