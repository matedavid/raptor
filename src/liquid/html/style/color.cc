#include "color.h"

namespace liquid {

Color color_from_string(const std::string& color_name)
{
  if (color_name == "transparent")
    return Color{255, 255, 255, 0.0};

  else if (color_name == "black")
    return Color{0,0,0};
  else if (color_name == "silver")
    return Color{192,192,192};
  else if (color_name == "gray" or color_name == "grey")
    return Color{128,128,128};
  else if (color_name == "white")
    return Color{255,255,255};
  else if (color_name == "maroon")
    return Color{128,0,0};
  else if (color_name == "red")
    return Color{255,0,0};
  else if (color_name == "purple")
    return Color{128,0,128};
  else if (color_name == "fuchsia")
    return Color{255,0,255};
  else if (color_name == "green")
    return Color{0,128,0};
  else if (color_name == "lime")
    return Color{0,255,0};
  else if (color_name == "olive")
    return Color{128,128,0};
  else if (color_name == "yellow")
    return Color{255,255,0};
  else if (color_name == "navy")
    return Color{0,0,128};
  else if (color_name == "blue")
    return Color{0,0,255};
  else if (color_name == "teal")
    return Color{0,128,128};
  else if (color_name == "aqua") 
    return Color{0,255,255};
  else if (color_name == "orange")
    return Color{255,165,0};

  return Color{0,0,0};
}

uint8_t hex_string_to_int(const std::string& hex_str)
{
  uint8_t value;
  std::stringstream ss;
  ss << std::hex << hex_str;
  ss >> value;

  return value;
}

Color parse_color(const std::string& value)
{
  std::regex rgb_regex = std::regex("rgb\\ *\\(\\ *([0-9]*)\\ *,\\ *([0-9]*)\\ *,\\ *([0-9]*),\\ *([0-9.]*)\\ *\\)");

  std::smatch match;
  bool rgb_regex_result = std::regex_match(value, match, rgb_regex);

  if (rgb_regex_result and match.size() == 4)
  {
    // regex without alpha
    uint8_t red   = std::atoi(match[1].str().c_str());
    uint8_t green = std::atoi(match[2].str().c_str());
    uint8_t blue  = std::atoi(match[3].str().c_str());

    return Color{red, green, blue};
  }
  else if (rgb_regex_result and match.size() == 5)
  {
    // regex with alpha
    uint8_t red   = std::atoi(match[1].str().c_str());
    uint8_t green = std::atoi(match[2].str().c_str());
    uint8_t blue  = std::atoi(match[3].str().c_str());
    float alpha   = std::atof(match[4].str().c_str());

    return Color{red, green, blue, alpha};
  }
	else if (value[0] == '#' and value.length() == 7) 
	{
    // hex format color
    std::string red_str   = value.substr(1, 2);
    std::string green_str = value.substr(3, 2);
    std::string blue_str  = value.substr(5, 2);

    int red   = std::stoi(red_str, nullptr, 16);
    int green = std::stoi(green_str, nullptr, 16);
    int blue  = std::stoi(blue_str, nullptr, 16);

    return Color{uint8_t(red), uint8_t(green), uint8_t(blue)};
	}

	return color_from_string(value);
}

}
