#include "utils.h"

float resolve_border_width(const std::string& border_width_value)
{
  if (border_width_value == "medium")
    return 3.0;
  else if (border_width_value == "thin")
    return 1.0;
  else if (border_width_value == "thick")
    return 5.0;

  if (is_number(border_width_value))
    return std::stof(border_width_value);

  return 0.;
}

bool is_number(const std::string& value)
{
  return value.find_first_not_of("0123456789.") == std::string::npos;
}
