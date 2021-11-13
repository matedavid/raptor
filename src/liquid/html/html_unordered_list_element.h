#pragma once

#include "liquid/html/html_element.h"

namespace liquid {

class HTMLUnorderedListElement : public HTMLElement
{
public:
	HTMLUnorderedListElement();
	using HTMLElement::HTMLElement;

  std::vector<HTMLElement*> get_list_elements();
};

}