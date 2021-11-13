#include "html_image_element.h"

namespace liquid {

HTMLImageElement::HTMLImageElement()
{
}

std::string HTMLImageElement::src()
{
  if (not contains_attribute("src"))
    return "";
  return get_attribute("src")->value();
}

std::string HTMLImageElement::alt()
{
  if (not contains_attribute("alt"))
    return "";
  return get_attribute("alt")->value();
}

}