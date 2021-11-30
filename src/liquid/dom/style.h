#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <regex>

#include <gtkmm/box.h>

#include "liquid/html/html_element.h"

#include "liquid/dom/config.h"

namespace liquid {

const int DEFAULT_FONT_SIZE = 16;

enum ParseNumberType
{
  Px,
  Em,
  Error
};

struct StyleNumber 
{
  float value;
  ParseNumberType type;
};

StyleNumber parse_number_value(const std::string& value);

static void add_margin_padding(Gtk::Box* box, const HTMLElement* element, const std::string& property);
static void add_margin_padding_side(Gtk::Box* box, const HTMLElement* element, const std::string& property);

void add_margin_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config);
void add_margin_side_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config);

void add_padding_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config);
void add_padding_side_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config);

void add_background_color_style(Gtk::Box* box, const HTMLElement* element);

}
