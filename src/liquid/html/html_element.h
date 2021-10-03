#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "liquid/html/token.h"
#include "liquid/html/attribute.h"

enum HTMLElementType
{
	ElementType,
	TextType
};

class HTMLElement
{
private:
	HTMLElement* parent;
	std::vector<HTMLElement*> children;

	std::map<std::string, Attribute*> attributes;

	std::string inner_html;
	std::string outer_html;

public:
	std::string element_value;
	std::string class_name;
	std::string id;

	virtual HTMLElementType type() { return HTMLElementType::ElementType; }

private:
	static HTMLElement* get_last_element_recursive(HTMLElement* element);

public:
	HTMLElement();
	HTMLElement(HTMLElement* parent);
	HTMLElement(const Token& token, HTMLElement* parent);

	HTMLElement* parent_element();
	std::vector<HTMLElement*> child_elements();

	// Inserts child to the current element
	void insert_child(HTMLElement* element);
	// Inserts child to the last element in the tree
	void insert_child_last(HTMLElement* element);
	void replace_child(const HTMLElement* element, const HTMLElement* child);
	void remove_child(const HTMLElement* element);
	bool contains(const HTMLElement* element) const;

	void set_parent(HTMLElement* new_parent);
	void insert_character(const std::string& str);

	std::vector<HTMLElement*> get_elements_by_tag_name(const std::string& tag);
	std::vector<HTMLElement*> get_elements_by_id(const std::string& id);
	std::vector<HTMLElement*> get_elements_by_class_name(const std::string& name);

	void set_attribute(const std::string& name, const std::string& value);
	void remove_attribute(const std::string& name);
	void toggle_attribute(const std::string& name);
	bool contains_attribute(const std::string& attribute_name);

	// Returns the last child of the provided element
	// The last child is the 'deepest' and right-most element in the tree
	HTMLElement* get_last_element();

	/* DEBUG */
	static void print_rec(HTMLElement* element, int tab_num);
	void print();
};
