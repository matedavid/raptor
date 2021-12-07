#pragma once

#include "liquid/html/html_element.h"

namespace liquid {

class HTMLItalicizedElement : public HTMLElement
{
public:
  HTMLItalicizedElement();
  using HTMLElement::HTMLElement;
};

}