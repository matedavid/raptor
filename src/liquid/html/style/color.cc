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
  else if (color_name == "gray")
    return Color{80,80,80};
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

Color parse_color(const std::string& value)
{
  std::regex rgb_regex = std::regex("rgb\\ *\\(\\ *([0-9]*)\\ *,\\ *([0-9]*)\\ *,\\ *([0-9]*),\\ *([0-9.]*)\\ *\\)");
  std::smatch match;

  bool rgb_regex_result = std::regex_match(value, match, rgb_regex);

  if (rgb_regex_result and match.size() == 4)
  {
    // regex without alpha
    uint8_t red = std::atoi(match[1].str().c_str());
    uint8_t green = std::atoi(match[2].str().c_str());
    uint8_t blue = std::atoi(match[3].str().c_str());

    return Color{red, green, blue};
  }
  else if (rgb_regex_result and match.size() == 5)
  {
    // regex with alpha
    uint8_t red = std::atoi(match[1].str().c_str());
    uint8_t green = std::atoi(match[2].str().c_str());
    uint8_t blue = std::atoi(match[3].str().c_str());
    float alpha = std::atof(match[4].str().c_str());

    return Color{red, green, blue, alpha};
  }
	else if (value[0] == '#')
	{
    
	}
	return color_from_string(value);
}

}
