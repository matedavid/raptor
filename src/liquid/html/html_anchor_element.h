#pragma once

#include "liquid/html/html_element.h"

namespace liquid {

class HTMLAnchorElement : public HTMLElement
{
public:
  HTMLAnchorElement();
  using HTMLElement::HTMLElement;

  // Returns the url that the anchor element links to
  std::string href();

  // Returns if the anchor element is active or not
  // TODO: Research how this is implemented
  bool is_active();

  // Returns the target of the anchor element (_self, _blank, _parent, _top)
  std::string target();
};

}