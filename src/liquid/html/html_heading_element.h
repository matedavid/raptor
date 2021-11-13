#pragma once

#include "liquid/html/html_element.h"

namespace liquid {

class HTMLHeadingElement : public HTMLElement
{
public:
  unsigned int level = 1;

  HTMLHeadingElement();
  using HTMLElement::HTMLElement;
};

}
