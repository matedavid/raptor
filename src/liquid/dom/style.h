#pragma once

#include <iostream>
#include <utility>
#include <regex>

#include <gtkmm/box.h>

#include "liquid/html/html_element.h"

namespace liquid {

const int DEFAULT_FONT_SIZE = 16;

enum ParseIntType
{
  Px,
  Em,
  Error
};

std::pair<int, ParseIntType> parse_int_value(const std::string& value);

void add_margin_style(Gtk::Box* box, const HTMLElement* element);
void add_margin_side_style(Gtk::Box* box, const HTMLElement* element);

void add_background_color_style(Gtk::Box* box, const HTMLElement* element);

}
