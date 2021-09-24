#include "html_element.h"

HTMLElement::HTMLElement()
{
}

HTMLElement::HTMLElement(HTMLElement* parent)
	: parent(parent)
{
}

HTMLElement::HTMLElement(const Token& token, HTMLElement* parent)
	: parent(parent)
{
	// TODO(david): Load information from token to HTMLElement
}
