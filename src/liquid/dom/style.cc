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

static void add_margin_padding(Gtk::Box* box, const HTMLElement* element, const std::string& property)
{
  std::string str = element->get_style_property_value(property)[0];
  std::pair<int, ParseIntType> value = parse_int_value(str);
  value.first *= PANGO_SCALE_LARGE;

  if (value.second == ParseIntType::Px)
  {
    box->set_margin_top(value.first);
    box->set_margin_right(value.first);
    box->set_margin_bottom(value.first);
    box->set_margin_left(value.first);
  }
  else if (value.second == ParseIntType::Em)
  {
    box->set_margin_top(value.first * DEFAULT_FONT_SIZE);
    box->set_margin_right(value.first * DEFAULT_FONT_SIZE);
    box->set_margin_bottom(value.first * DEFAULT_FONT_SIZE);
    box->set_margin_left(value.first * DEFAULT_FONT_SIZE);
  }
}

static void add_margin_padding_side(Gtk::Box* box, const HTMLElement* element, const std::string& property)
{
  std::string top = property + "-top";
  std::string right = property + "-right";
  std::string bottom = property + "-bottom";
  std::string left = property + "-left";

  if (element->contains_style(top))
  {
    std::string str = element->get_style_property_value(top)[0];
    std::pair<int, ParseIntType> value = parse_int_value(str);
    value.first *= PANGO_SCALE_LARGE;

    if (value.second == ParseIntType::Px)
      box->set_margin_top(value.first);
    else if (value.second == ParseIntType::Em)
      box->set_margin_top(value.first * DEFAULT_FONT_SIZE);
  }

  if (element->contains_style(right))
  {
    std::string str = element->get_style_property_value(right)[0];
    std::pair<int, ParseIntType> value = parse_int_value(str);
    value.first *= PANGO_SCALE_LARGE;

    if (value.second == ParseIntType::Px)
      box->set_margin_right(value.first);
    else if (value.second == ParseIntType::Em)
      box->set_margin_right(value.first * DEFAULT_FONT_SIZE);
  }

  if (element->contains_style(bottom))
  {
    std::string str = element->get_style_property_value(bottom)[0];
    std::pair<int, ParseIntType> value = parse_int_value(str);
    value.first *= PANGO_SCALE_LARGE;

    if (value.second == ParseIntType::Px)
      box->set_margin_bottom(value.first);
    else if (value.second == ParseIntType::Em)
      box->set_margin_bottom(value.first * DEFAULT_FONT_SIZE);
  }

  if (element->contains_style(left))
  {
    std::string str = element->get_style_property_value(left)[0];
    std::pair<int, ParseIntType> value = parse_int_value(str);
    value.first *= PANGO_SCALE_LARGE;

    if (value.second == ParseIntType::Px)
      box->set_margin_left(value.first);
    else if (value.second == ParseIntType::Em)
      box->set_margin_left(value.first * DEFAULT_FONT_SIZE);
  }
}

void add_margin_style(Gtk::Box* box, const HTMLElement* element)
{
  add_margin_padding(box, element, "margin");
}

void add_margin_side_style(Gtk::Box* box, const HTMLElement* element)
{
  add_margin_padding_side(box, element, "margin");
}

void add_padding_style(Gtk::Box* box, const HTMLElement* element)
{
  add_margin_padding(box, element, "padding");
}

void add_padding_side_style(Gtk::Box* box, const HTMLElement* element)
{
  add_margin_padding_side(box, element, "padding");
}

void add_background_color_style(Gtk::Box* box, const HTMLElement* element)
{
  std::string background_color_str = element->get_style_property_value("background-color")[0];
  box->override_background_color(Gdk::RGBA(background_color_str));
}

}
