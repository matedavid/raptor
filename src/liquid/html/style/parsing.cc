#include "parsing.h"

CSSNumber parse_number(const std::string& value)
{
  // Finds values in a format like: 16px -> '16' and 'px'
  std::regex regexexpr("([0-9^\\.?]*)([a-zA-Z%]*)");

  // First number
  std::smatch m;
  std::regex_search(value, m, regexexpr);

  // Means that contains both number and format 
  if (m.size() != 3)
    return CSSNumber{0.0, CSSNumberType::Error};

  // Convert m[1] value to float
  std::stringstream ss;
  float value_number;

  ss << m[1].str();
  ss >> value_number;

  std::string type_str = m[2];

  CSSNumberType type;
  if (type_str == "px")
    type = CSSNumberType::Px;
  else if (type_str == "em")
    type = CSSNumberType::Em;
  else if (type_str == "%")
    type = CSSNumberType::Percentage;
  else 
    type = CSSNumberType::Error;

  return CSSNumber{value_number, type};
}

/*
std::pair<CSSNumber, CSSNumber> parse_pair_numbers(const std::vector<std::string>& value, int i)
{
  CSSNumber num1 = parse_number(value[i]);
  CSSNumber num2 = parse_number(value[i+1]);

  if (num1.type != num2.type and num1.type != CSSNumberType::Error and num2.type != CSSNumberType::Error)
    return std::make_pair(CSSNumber{0.0, CSSNumberType::Error}, CSSNumber{0.0, CSSNumberType::Error});
  else if (num1.type == CSSNumberType::Error or num2.type == CSSNumberType::Error)
    return std::make_pair(CSSNumber{0.0, CSSNumberType::Error}, CSSNumber{0.0, CSSNumberType::Error});
  
  return std::make_pair(num1, num2);
}
*/

float get_css_number(CSSNumber& n, float& em_reference_value)
{
  if (n.type == CSSNumberType::Px)
    return n.value;
  else if (n.type == CSSNumberType::Em)
    return n.value*em_reference_value;
  else 
    return 0.0;
}

bool is_property_inherited(const std::string& property)
{
  if (property == "font" or
      property == "font-style" or
      property == "font-variant" or
      property == "font-weight" or
      property == "font-size" or 
      property == "line-height" or
      property == "font-family" or
      property == "text-decoration" or      // text-decoration (+ individual values) does not have inherited property, but if not will not cascase down to Text child
      property == "text-decoration-line" or
      property == "text-decoration-color" or
      property == "text-decoration-style")
  {
    return true;
  }
  return false;
}

void print_style(const HTMLElementCSSValues& style)
{
  std::cout << "font-style: " << style.font_style << std::endl;
  std::cout << "font-variant: " << style.font_variant << std::endl;
  std::cout << "font-weight: " << style.font_weight << std::endl;
  std::cout << "font-size: " << style.font_size << std::endl;
  std::cout << "font-font_family: " << style.font_family << std::endl;

  std::cout << std::endl;

  std::cout << "background-color: " << style.background_color << std::endl;
  std::cout << "background-image: " << style.background_image << std::endl;
  std::cout << "background-position: (" << style.background_position.first << ", " << style.background_position.second << ")" << std::endl;
  //std::cout << "background-size: " << style.background_size << std::endl;
  std::cout << "background-repeat: " << style.background_repeat << std::endl;
  std::cout << "background-origin: " << style.background_origin << std::endl;
  std::cout << "background-clip: " << style.background_clip << std::endl;
  std::cout << "background-attachment: " << style.background_attachment << std::endl;

  std::cout << std::endl;

  std::cout << "margin-top: " << style.margin_top << std::endl;
  std::cout << "margin-bottom: " << style.margin_bottom << std::endl;
  std::cout << "margin-right: " << style.margin_right << std::endl;
  std::cout << "margin-left: " << style.margin_left << std::endl;

  std::cout << std::endl;

  std::cout << "padding-top: " << style.padding_top << std::endl;
  std::cout << "padding-bottom: " << style.padding_bottom << std::endl;
  std::cout << "padding-right: " << style.padding_right << std::endl;
  std::cout << "padding-left: " << style.padding_left << std::endl;

  std::cout << std::endl;
}