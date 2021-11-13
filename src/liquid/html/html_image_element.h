#pragma once

#include "liquid/html/html_element.h"

namespace liquid {

class HTMLImageElement : public HTMLElement
{
public:
	HTMLImageElement();
	using HTMLElement::HTMLElement;

  // Returns src attribute from image 
  std::string src();

  // Returns the alt attribute (description of image)
  std::string alt();
};

}
