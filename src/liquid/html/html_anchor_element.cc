#include "html_anchor_element.h"

namespace liquid {

HTMLAnchorElement::HTMLAnchorElement()
{
}

std::string HTMLAnchorElement::href()
{
  if (not contains_attribute("href"))
    return "";
  return get_attribute("href")->value();
}

bool HTMLAnchorElement::is_active()
{
  // TODO: Implement this function (research it)
}

std::string HTMLAnchorElement::target()
{
  if (not contains_attribute("target"))
    return "";
  return get_attribute("target")->value();
}

}