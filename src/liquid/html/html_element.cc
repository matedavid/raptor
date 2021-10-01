#include "html_element.h"

HTMLElement* HTMLElement::get_last_element(HTMLElement* element)
{
	if (element == nullptr)
		return nullptr;

	int size = element->children.size();
	if (size == 0)
		return element;

	return get_last_element(element->children[size-1]);
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
}

void HTMLElement::insert_child(HTMLElement* element)
{
	element->set_parent(this);
	children.push_back(element);
}

void HTMLElement::insert_child_last(HTMLElement* element)
{
	HTMLElement* last_element = get_last_element(this);
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
	attributes.insert(make_pair(name, attr));
}


/* DEBUG */
void HTMLElement::print_rec(HTMLElement* element, int tab_num)
{
	if (element == nullptr)
		return;

	for (int t = 0; t < tab_num; ++t) std::cout << "  ";

	// Print information of element
	std::cout << "[" << element->element_value;
	auto attr_it = element->attributes.begin();
	while (attr_it != element->attributes.end())
	{
		Attribute* attr = attr_it->second;
		std::cout << " " << attr->name() << ":" << attr->value();
		++attr_it;
	}
	std::cout << "]" << std::endl;

	for (int i = 0; i < element->children.size(); ++i)
		print_rec(element->children[i], tab_num+1);
}

void HTMLElement::print()
{
	print_rec(this, 0);
}
