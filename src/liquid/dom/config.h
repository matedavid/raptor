#pragma once

#include <vector>
#include <utility>

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

RenderBox* new_render_box(const std::string& element_value, Gtk::Orientation orientation);

struct RenderConfig 
{
  // Lists
  bool list;                  // If element is under a list
  unsigned int list_type;     // ul: 0 - ol: 1
  int unordered_list_marker;  // TODO: Not implemented, should decide how to represent markers

  // Box where the marker from a list element goes
  Gtk::Box* list_marker_box;
};

struct RenderInfo 
{
  // List of all rendered anchor elements, with a pointer to the widget and a string with the target
  std::vector<Gtk::LinkButton*> anchor_elements;
};

}
