#pragma once

#include <iostream>

#include "liquid/html/html_element.h"

namespace liquid {

class HTMLHeadElement : public HTMLElement
{
public:
	HTMLHeadElement();
	using HTMLElement::HTMLElement;
};

}
