#include "style.h"

namespace liquid {

std::pair<int, ParseIntType> parse_int_value(const std::string& value)
{
  // Finds values in a format like: 16px -> '16' and 'px'
  std::regex regexexpr("([0-9]*)([a-zA-Z]*)");

  std::smatch m;
  std::regex_search(value, m, regexexpr);

  // Means that contains both number and format 
  if (m.size() != 3)
    return std::make_pair(-1, ParseIntType::Error);
  
  int value_number = std::atoi(m[1].str().c_str());
  std::string type_str = m[2];

  ParseIntType type;
  if (type_str == "px")
    type = ParseIntType::Px;
  else if (type_str == "em")
    type = ParseIntType::Em;
  else 
    type = ParseIntType::Error;

  return std::make_pair(value_number, type);
}

void add_margin_style(Gtk::Box* box, const HTMLElement* element)
{
  std::string margin_str = element->get_style_property_value("margin")[0];
  std::pair<int, ParseIntType> margin_value = parse_int_value(margin_str);

  if (margin_value.second == ParseIntType::Px)
  {
    box->set_margin_top(margin_value.first);
    box->set_margin_right(margin_value.first);
    box->set_margin_bottom(margin_value.first);
    box->set_margin_left(margin_value.first);
  }
  else if (margin_value.second == ParseIntType::Em)
  {
    box->set_margin_top(margin_value.first * DEFAULT_FONT_SIZE);
    box->set_margin_right(margin_value.first * DEFAULT_FONT_SIZE);
    box->set_margin_bottom(margin_value.first * DEFAULT_FONT_SIZE);
    box->set_margin_left(margin_value.first * DEFAULT_FONT_SIZE);
  }
}

void add_margin_top_style(Gtk::Box* box, const HTMLElement* element)
{

}

void add_margin_right_style(Gtk::Box* box, const HTMLElement* element)
{

}

void add_margin_bottom_style(Gtk::Box* box, const HTMLElement* element)
{

}

void add_margin_left_style(Gtk::Box* box, const HTMLElement* element)
{

}

}