#pragma once

#include "liquid/html/html_element.h"

namespace liquid {

class HTMLOrderedListElement : public HTMLElement
{
public:
	HTMLOrderedListElement();
	using HTMLElement::HTMLElement;

  std::vector<HTMLElement*> get_list_elements();
};

}
