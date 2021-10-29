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

void add_margin_side_style(Gtk::Box* box, const HTMLElement* element)
{
  if (element->contains_style("margin-top"))
  {
    std::string margin_top_str = element->get_style_property_value("margin-top")[0];
    std::pair<int, ParseIntType> margin_top_value = parse_int_value(margin_top_str);
    if (margin_top_value.second == ParseIntType::Px)
      box->set_margin_top(margin_top_value.first);
    else if (margin_top_value.second == ParseIntType::Em)
      box->set_margin_top(margin_top_value.first * DEFAULT_FONT_SIZE);
  }

  if (element->contains_style("margin-right"))
  {
    std::string margin_right_str = element->get_style_property_value("margin-right")[0];
    std::pair<int, ParseIntType> margin_right_value = parse_int_value(margin_right_str);
    if (margin_right_value.second == ParseIntType::Px)
      box->set_margin_right(margin_right_value.first);
    else if (margin_right_value.second == ParseIntType::Em)
      box->set_margin_right(margin_right_value.first * DEFAULT_FONT_SIZE);
  }

  if (element->contains_style("margin-bottom"))
  {
    std::string margin_bottom_str = element->get_style_property_value("margin-bottom")[0];
    std::pair<int, ParseIntType> margin_bottom_value = parse_int_value(margin_bottom_str);
    if (margin_bottom_value.second == ParseIntType::Px)
      box->set_margin_bottom(margin_bottom_value.first);
    else if (margin_bottom_value.second == ParseIntType::Em)
      box->set_margin_bottom(margin_bottom_value.first * DEFAULT_FONT_SIZE);
  }

  if (element->contains_style("margin-left"))
  {
    std::string margin_left_str = element->get_style_property_value("margin-left")[0];
    std::pair<int, ParseIntType> margin_left_value = parse_int_value(margin_left_str);
    if (margin_left_value.second == ParseIntType::Px)
      box->set_margin_left(margin_left_value.first);
    else if (margin_left_value.second == ParseIntType::Em)
      box->set_margin_left(margin_left_value.first * DEFAULT_FONT_SIZE);
  }
}

void add_background_color_style(Gtk::Box* box, const HTMLElement* element)
{
  std::string background_color_str = element->get_style_property_value("background-color")[0];
  box->override_background_color(Gdk::RGBA(background_color_str));
}

}