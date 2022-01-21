#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <filesystem>

#include "liquid/html/token.h"
#include "liquid/html/attribute.h"
#include "liquid/html/html_element_css_values.h"
#include "liquid/css/parser.h"

namespace liquid {

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
	std::map< std::string, std::vector<std::string> > styles;

public:
	std::string element_value;
	std::string class_name;
	std::string id;

	std::string inner_html;
	std::string outer_html;

	virtual HTMLElementType type() const { return HTMLElementType::ElementType; }

private:
	static void replace_child_recursive(HTMLElement* element, HTMLElement* target, HTMLElement* new_child);
	static void remove_child_recursive(HTMLElement* element, HTMLElement* target);

	static void get_elements_by_tag_name_recursive(HTMLElement* element, const std::string& tag, std::vector<HTMLElement*>& element_list);
	static void get_elements_by_class_name_recursive(HTMLElement* element, const std::string& name, std::vector<HTMLElement*>& element_list);
	static void get_elements_by_id_recursive(HTMLElement* element, const std::string& name, std::vector<HTMLElement*>& element_list);

	static HTMLElement* get_last_element_recursive(HTMLElement* element);

public:
	HTMLElement();
	HTMLElement(HTMLElement* parent);
	HTMLElement(const Token& token, HTMLElement* parent, const std::filesystem::path& document_path);

	HTMLElement* parent_element();
	std::vector<HTMLElement*> child_elements();

	// Inserts child to the current element
	void insert_child(HTMLElement* element);
	// Inserts child to the last element in the tree
	void insert_child_last(HTMLElement* element);
	void replace_child(HTMLElement* element, HTMLElement* new_child);
	void remove_child(HTMLElement* element);
	bool contains(const HTMLElement* element) const;

	void set_parent(HTMLElement* new_parent);

	std::vector<HTMLElement*> get_elements_by_tag_name(const std::string& tag);
	std::vector<HTMLElement*> get_elements_by_id(const std::string& id);
	std::vector<HTMLElement*> get_elements_by_class_name(const std::string& name);

	void set_attribute(const std::string& name, const std::string& value);
	Attribute* get_attribute(const std::string& name) const;
	void remove_attribute(const std::string& name);
	void toggle_attribute(const std::string& name);
	bool contains_attribute(const std::string& name) const;

	void apply_css(const std::map<std::string, std::vector<std::string>>& declarations);

	void set_style_property(const std::string& property, const std::string& value);
	void set_style_property(const std::string& property, const std::vector<std::string>& value);
	std::vector<std::string> get_style_property_value(const std::string& property) const;
	bool contains_style(const std::string& property) const;

	// Returns the last child of the provided element
	// The last child is the 'deepest' and right-most element in the tree
	HTMLElement* get_last_element();
  
  /* === DEBUG === */
	std::map<std::string, Attribute*> get_attributes() { return attributes; }
	int number_style_properties() const { return styles.size(); }
};

}
