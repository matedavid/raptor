#include "style.h"

namespace liquid {

/*
StyleNumber parse_number_value(const std::string& value)
{
  // Finds values in a format like: 16px -> '16' and 'px'
  std::regex regexexpr("([0-9^\\.?]*)([a-zA-Z]*)");

  std::smatch m;
  std::regex_search(value, m, regexexpr);

  // Means that contains both number and format 
  if (m.size() != 3)
    return StyleNumber{-1, ParseNumberType::Error};
  
  // Convert m[1] value to float
  std::stringstream ss;
  float value_number;

  ss << m[1].str();
  ss >> value_number;

  std::string type_str = m[2];

  ParseNumberType type;
  if (type_str == "px")
    type = ParseNumberType::Px;
  else if (type_str == "em")
    type = ParseNumberType::Em;
  else 
    type = ParseNumberType::Error;

  return StyleNumber{value_number, type};
}

static void add_margin_padding(Gtk::Box* box, const HTMLElement* element, const std::string& property, const RenderConfig& config)
{
  std::string str = element->get_style_property_value(property)[0];
  StyleNumber value = parse_number_value(str);
  //value.value *= PANGO_SCALE_X_SMALL;

  if (value.type == ParseNumberType::Px)
  {
    box->set_margin_top(value.value);
    box->set_margin_right(value.value);
    box->set_margin_bottom(value.value);
    box->set_margin_left(value.value);
  }
  else if (value.type == ParseNumberType::Em)
  {
    box->set_margin_top(value.value * config.font_size * PANGO_SCALE_X_SMALL);
    box->set_margin_right(value.value * config.font_size * PANGO_SCALE_X_SMALL);
    box->set_margin_bottom(value.value * config.font_size * PANGO_SCALE_X_SMALL);
    box->set_margin_left(value.value * config.font_size * PANGO_SCALE_X_SMALL);
  }
}

static void add_margin_padding_side(Gtk::Box* box, const HTMLElement* element, const std::string& property, const RenderConfig& config)
{
  std::string top = property + "-top";
  std::string right = property + "-right";
  std::string bottom = property + "-bottom";
  std::string left = property + "-left";

  if (element->contains_style(top))
  {
    std::string str = element->get_style_property_value(top)[0];
    StyleNumber value = parse_number_value(str);
    //value.value *= PANGO_SCALE_X_SMALL;

    if (value.type == ParseNumberType::Px)
      box->set_margin_top(value.value);
    else if (value.type == ParseNumberType::Em)
      box->set_margin_top(value.value * config.font_size * PANGO_SCALE_X_SMALL);
  }

  if (element->contains_style(right))
  {
    std::string str = element->get_style_property_value(right)[0];
    StyleNumber value = parse_number_value(str);
    //value.value *= PANGO_SCALE_X_SMALL;

    if (value.type == ParseNumberType::Px)
      box->set_margin_right(value.value);
    else if (value.type == ParseNumberType::Em)
      box->set_margin_right(value.value * config.font_size * PANGO_SCALE_X_SMALL);
  }

  if (element->contains_style(bottom))
  {
    std::string str = element->get_style_property_value(bottom)[0];
    StyleNumber value = parse_number_value(str);
    //value.value *= PANGO_SCALE_X_SMALL;

    if (value.type == ParseNumberType::Px)
      box->set_margin_bottom(value.value);
    else if (value.type == ParseNumberType::Em)
      box->set_margin_bottom(value.value * config.font_size * PANGO_SCALE_X_SMALL);
  }

  if (element->contains_style(left))
  {
    std::string str = element->get_style_property_value(left)[0];
    StyleNumber value = parse_number_value(str);
    //value.value *= PANGO_SCALE_X_SMALL;

    if (value.type == ParseNumberType::Px)
      box->set_margin_left(value.value);
    else if (value.type == ParseNumberType::Em)
      box->set_margin_left(value.value * config.font_size * PANGO_SCALE_X_SMALL);
  }
}

void add_margin_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config)
{
  add_margin_padding(box, element, "margin", config);
}

void add_margin_side_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config)
{
  add_margin_padding_side(box, element, "margin", config);
}

void add_padding_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config)
{
  add_margin_padding(box, element, "padding", config);
}

void add_padding_side_style(Gtk::Box* box, const HTMLElement* element, const RenderConfig& config)
{
  add_margin_padding_side(box, element, "padding", config);
}

void add_background_color_style(Gtk::Box* box, const HTMLElement* element)
{
  std::string background_color_str = element->get_style_property_value("background-color")[0];
  box->override_background_color(Gdk::RGBA(background_color_str));
}
*/

}
