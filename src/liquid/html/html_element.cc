#include "html_element.h"

namespace liquid {

void HTMLElement::replace_child_recursive(HTMLElement* element, HTMLElement* target, HTMLElement* new_child)
{
	if (element->children.size() == 0)
		return;

	for (int i = 0; i < element->children.size(); ++i)
	{
		if (element->children[i] == target)
		{
			// TODO: this is probably a memory leak, not deleting childs of this element
			element->children[i] = new_child;
		}
	}
}

void HTMLElement::remove_child_recursive(HTMLElement* element, HTMLElement* target)
{
	if (element->children.size() == 0)
		return;

	std::vector<HTMLElement*>::iterator iter = element->children.begin();
	while (iter != element->children.end())
	{
		if (*iter == target)
		{
			element->children.erase(iter);
			return;
		}
		++iter;
	}

	for (int i = 0; i < element->children.size(); ++i)
		remove_child_recursive(element->children[i], target);
}

void HTMLElement::get_elements_by_tag_name_recursive(HTMLElement* element, const std::string& tag, std::vector<HTMLElement*>& element_list)
{
	if (element->element_value == tag)
		element_list.push_back(element);

	if (element->children.size() == 0)
		return;

	for (int i = 0; i < element->children.size(); ++i)
		get_elements_by_tag_name_recursive(element->children[i], tag, element_list);
}

void HTMLElement::get_elements_by_class_name_recursive(HTMLElement* element, const std::string& name, std::vector<HTMLElement*>& element_list)
{
	if (element->class_name == name)
		element_list.push_back(element);

	if (element->children.size() == 0)
		return;

	for (int i = 0; i < element->children.size(); ++i)
		get_elements_by_class_name_recursive(element->children[i], name, element_list);
}

void HTMLElement::get_elements_by_id_recursive(HTMLElement* element, const std::string& id, std::vector<HTMLElement*>& element_list)
{
	if (element->id == id)
		element_list.push_back(element);

	if (element->children.size() == 0)
		return;

	for (int i = 0; i < element->children.size(); ++i)
		get_elements_by_id_recursive(element->children[i], id, element_list);
}

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

		if (attribute->name() == "class")
			class_name = attribute->value();
		else if (attribute->name() == "id")
			id = attribute->value();
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

void HTMLElement::replace_child(HTMLElement* element, HTMLElement* new_child)
{
	replace_child_recursive(this, element, new_child);
}

void HTMLElement::remove_child(HTMLElement* element)
{
	remove_child_recursive(this, element);
}


void HTMLElement::set_parent(HTMLElement* new_parent)
{
	parent = new_parent;
}

std::vector<HTMLElement*> HTMLElement::get_elements_by_tag_name(const std::string& tag)
{
	std::vector<HTMLElement*> element_list;
	get_elements_by_tag_name_recursive(this, tag, element_list);
	return element_list;
}

std::vector<HTMLElement*> HTMLElement::get_elements_by_class_name(const std::string& name)
{
	std::vector<HTMLElement*> element_list;
	get_elements_by_class_name_recursive(this, name, element_list);
	return element_list;
}

std::vector<HTMLElement*> HTMLElement::get_elements_by_id(const std::string& id)
{
	std::vector<HTMLElement*> element_list;
	get_elements_by_id_recursive(this, id, element_list);
	return element_list;
}

void HTMLElement::set_attribute(const std::string& name, const std::string& value)
{
	Attribute* attr = new Attribute(name, value);
	attributes.insert(std::make_pair(name, attr));
}

Attribute* HTMLElement::get_attribute(const std::string& name) const
{
	if (contains_attribute(name))
		return attributes.find(name)->second;
	return nullptr;
}

bool HTMLElement::contains_attribute(const std::string& name) const
{
	return attributes.find(name) != attributes.end();
}

void HTMLElement::set_style_property(const std::string& property, const std::string& value)
{
	if (contains_style(property))
		return;
	styles.insert(std::make_pair(property, std::vector<std::string>{value}));
}


void HTMLElement::set_style_property(const std::string& property, const std::vector<std::string>& value)
{
	if (contains_style(property))
		return;
	styles.insert(std::make_pair(property, value));
}

std::vector<std::string> HTMLElement::get_style_property_value(const std::string& property) const
{
	if (not contains_style(property))
		return std::vector<std::string>{};
	return styles.find(property)->second;
}

bool HTMLElement::contains_style(const std::string& property) const
{
	return styles.find(property) != styles.end();
}

HTMLElement* HTMLElement::get_last_element()
{
	return get_last_element_recursive(this);
}

}
