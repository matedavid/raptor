#include "html_element.h"

HTMLElement* HTMLElement::get_last_element_recursive(HTMLElement* element)
{
	if (element == nullptr)
		return nullptr;

	int size = element->children.size();
	if (size == 0)
		return element;

	return get_last_element_recursive(element->children[size-1]);
}

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
	element_value = token.value;

  for (int i = 0; i < token.attributes.size(); ++i)
  {
    Attr attr = token.attributes[i];

    Attribute* attribute = new Attribute(attr.first, attr.second);
    attributes.insert(std::make_pair(attr.first, attribute));
  }
}

HTMLElement* HTMLElement::parent_element()
{
	return parent;
}

std::vector<HTMLElement*> HTMLElement::child_elements()
{
	return children;
}

void HTMLElement::insert_child(HTMLElement* element)
{
	element->set_parent(this);
	children.push_back(element);
}

void HTMLElement::insert_child_last(HTMLElement* element)
{
	HTMLElement* last_element = get_last_element();
	if (last_element != nullptr)
		last_element->insert_child(element);
}

void HTMLElement::set_parent(HTMLElement* new_parent)
{
	parent = new_parent;
}

void HTMLElement::insert_character(const std::string& str)
{

}

void HTMLElement::set_attribute(const std::string& name, const std::string& value)
{
	Attribute* attr = new Attribute(name, value);
	attributes.insert(std::make_pair(name, attr));
}

HTMLElement* HTMLElement::get_last_element()
{
	return get_last_element_recursive(this);
}

