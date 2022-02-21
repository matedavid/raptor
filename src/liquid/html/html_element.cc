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

	//for (int i = 0; i < element->children.size(); ++i)
	for (auto child : element->children)
		get_elements_by_tag_name_recursive(child, tag, element_list);
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

HTMLElement::HTMLElement(const Token& token, HTMLElement* parent, const std::filesystem::path& document_path)
	: parent(parent)
{
	element_value = token.value;

  for (int i = 0; i < token.attributes.size(); ++i)
  {
    Attr attr = token.attributes[i];

    Attribute* attribute = new Attribute(attr.first, attr.second);
    attributes.insert(std::make_pair(attr.first, attribute));

		if (attribute->name() == "src" and attribute->value()[0] != '/')
		{
			std::string absolute_path = (document_path/attribute->value()).string();
			attribute->set_value(absolute_path);
		}

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

void HTMLElement::apply_css(const std::map<std::string, std::vector<std::string>>& declarations)
{
	for (auto declaration : declarations)
		set_style_property(declaration.first, declaration.second);
}	

/*
void HTMLElement::set_style_property(const std::string& property, const std::string& value)
{
	if (contains_style(property))
	{
		std::vector<std::string> v = {value};
		styles[property] = v; 
	}
	styles.insert(std::make_pair(property, std::vector<std::string>{value}));
}
*/

void HTMLElement::set_style_property(const std::string& property, const std::vector<std::string>& value)
{
	if (property == "color")
	{
		style.color = value[0];
	}
	else if (property == "width")
	{
		if (value[0] == "auto")
			style.width = -1;
		else if (is_value_css_number(value[0]))
		{
			CSSNumber width = parse_number(value[0]);
			style.width = get_css_number(width, style.font_size);
		}
	}
	else if (property == "height")
	{
		if (value[0] == "auto")
			style.height = -1;
		else if (is_value_css_number(value[0]))
		{
			CSSNumber height = parse_number(value[0]);
			style.height = get_css_number(height, style.font_size);
		}
	}

	// Font
	else if (property == "font")
	{
	}
	else if (property == "font-style")
	{
		style.font_style = value[0];
	}
	else if (property == "font-variant")
	{
		style.font_variant = value[0];
	}
	else if (property == "font-weight")
	{
		style.font_weight = value[0];
	}
	else if (property == "font-size")
	{
		CSSNumber n_value = parse_number(value[0]);

		if (n_value.type == CSSNumberType::Px)
			style.font_size = n_value.value;
		else if (n_value.type == CSSNumberType::Em and parent != nullptr)
			style.font_size = get_css_number(n_value, parent->style.font_size);
		else if (n_value.type == CSSNumberType::Em and parent == nullptr)
			style.font_size = get_css_number(n_value, style.font_size);
	}
	else if (property == "font-family")
	{
		style.font_family = value[0];
	}

	// Background
	else if (property == "background")
	{

	}
	else if (property == "background-color")
	{
		style.background_color = value[0];
	}
	else if (property == "background-image")
	{
		style.background_image = value[0];
	}
	else if (property == "background-position")
	{
		if (value.size() >= 2)
		{
			CSSNumber num1 = parse_number(value[0]);
			CSSNumber num2 = parse_number(value[1]);

			if (num1.type != CSSNumberType::Error and num2.type != CSSNumberType::Error)
			{
				style.background_position.first = num1.type == CSSNumberType::Px 
                                          ? std::to_string(num1.value)
																					: std::to_string(num1.value*style.font_size);
				style.background_position.second = num2.type == CSSNumberType::Px 
																					? std::to_string(num2.value)
																					: std::to_string(num2.value*style.font_size);
			}
			else if (num1.type != CSSNumberType::Error)
			{
				style.background_position.first = num1.type == CSSNumberType::Px 
																					? std::to_string(num1.value)
																					: std::to_string(num1.value*style.font_size);
				style.background_position.second = num1.value;
			}
			else if (num2.type != CSSNumberType::Error)
			{
				style.background_position.first = num2.value;
				style.background_position.second = num2.type == CSSNumberType::Px 
																					? std::to_string(num2.value) 
																					: std::to_string(num2.value*style.font_size);
			}
			else
			{
				style.background_position.first = value[0];
				style.background_position.second = value[1];
			}
		}
		else if (value.size() == 1)
		{
			CSSNumber num = parse_number(value[0]);
			if (num.type != CSSNumberType::Error)
			{
				style.background_position.first = num.type == CSSNumberType::Px 
																				? std::to_string(num.value)
																				: std::to_string(num.value*style.font_size);
				style.background_position.second = std::to_string(num.value);
			}
			else
			{
				style.background_position.first = value[0];
				style.background_position.second = "center";
			}
		}
	}
	else if (property == "background-size")
	{
		// TODO
	}
	else if (property == "background-repeat")
	{
		style.background_repeat = value[0];
	}
	else if (property == "background-origin")
	{
		style.background_origin = value[0];
	}
	else if (property == "background-clip")
	{
		style.background_clip = value[0];
	}
	else if (property == "background-attachment")
	{
		style.background_attachment = value[0];
	}

	// Margin
	else if (property == "margin")
	{
		if (value.size() == 4)
		{
			CSSNumber number1 = parse_number(value[0]);
			CSSNumber number2 = parse_number(value[1]);
			CSSNumber number3 = parse_number(value[2]);
			CSSNumber number4 = parse_number(value[3]);

			style.margin_top = get_css_number(number1, style.font_size);
			style.margin_right = get_css_number(number2, style.font_size);
			style.margin_bottom = get_css_number(number3, style.font_size);
			style.margin_left = get_css_number(number4, style.font_size);
		}
		else if (value.size() == 3)
		{
			CSSNumber number1 = parse_number(value[0]);
			CSSNumber number2 = parse_number(value[1]);
			CSSNumber number3 = parse_number(value[2]);

			style.margin_top = get_css_number(number1, style.font_size);
			style.margin_right = style.margin_left = get_css_number(number2, style.font_size);
			style.margin_bottom = get_css_number(number3, style.font_size);
		}
		else if (value.size() == 2)
		{
			CSSNumber number1 = parse_number(value[0]);
			CSSNumber number2 = parse_number(value[1]);

			style.margin_top = style.margin_bottom = get_css_number(number1, style.font_size);
			style.margin_right = style.margin_left = get_css_number(number2, style.font_size);
		}
		else
		{
			CSSNumber number = parse_number(value[0]);
			style.margin_top = style.margin_bottom = style.margin_left = style.margin_right = get_css_number(number, style.font_size);
		}
	}
	else if (property == "margin-top")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.margin_top = get_css_number(n_value, style.font_size);
	}
	else if (property == "margin-bottom")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.margin_bottom = get_css_number(n_value, style.font_size);
	}
	else if (property == "margin-left")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.margin_left = get_css_number(n_value, style.font_size);
	}
	else if (property == "margin-right")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.margin_right = get_css_number(n_value, style.font_size);
	}
	
	// Padding 
	else if (property == "padding")
	{
		if (value.size() == 4)
		{
			CSSNumber number1 = parse_number(value[0]);
			CSSNumber number2 = parse_number(value[1]);
			CSSNumber number3 = parse_number(value[2]);
			CSSNumber number4 = parse_number(value[3]);

			style.padding_top = get_css_number(number1, style.font_size);
			style.padding_right = get_css_number(number2, style.font_size);
			style.padding_bottom = get_css_number(number3, style.font_size);
			style.padding_left = get_css_number(number4, style.font_size);
		}
		else if (value.size() == 3)
		{
			CSSNumber number1 = parse_number(value[0]);
			CSSNumber number2 = parse_number(value[1]);
			CSSNumber number3 = parse_number(value[2]);

			style.padding_top = get_css_number(number1, style.font_size);
			style.padding_right = style.padding_left = get_css_number(number2, style.font_size);
			style.padding_bottom = get_css_number(number3, style.font_size);
		}
		else if (value.size() == 2)
		{
			CSSNumber number1 = parse_number(value[0]);
			CSSNumber number2 = parse_number(value[1]);

			style.padding_top = style.padding_bottom = get_css_number(number1, style.font_size);
			style.padding_right = style.padding_left = get_css_number(number2, style.font_size);
		}
		else
		{
			CSSNumber number = parse_number(value[0]);
			style.padding_top = style.padding_bottom = style.padding_left = style.padding_right = get_css_number(number, style.font_size);
		}
	}
	else if (property == "padding-top")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.padding_top = get_css_number(n_value, style.font_size);
	}
	else if (property == "padding-bottom")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.padding_bottom = get_css_number(n_value, style.font_size);
	}
	else if (property == "padding-left")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.padding_left = get_css_number(n_value, style.font_size);
	}
	else if (property == "padding-right")
	{
		CSSNumber n_value = parse_number(value[0]);
		style.padding_right = get_css_number(n_value, style.font_size);
	}

	// Text decoration
	else if (property == "text-decoration")
	{
		style.text_decoration_line.clear();

		for (const std::string& text_decoration : value)
		{
			if (text_decoration == "underline" or text_decoration == "overline" or text_decoration == "line-through" or text_decoration == "none")
				style.text_decoration_line.push_back(text_decoration);
			else if (text_decoration == "solid" or text_decoration == "double" or text_decoration == "dotted" or text_decoration == "dashed" or text_decoration == "wavy")
				style.text_decoration_style = text_decoration;
			else
				style.text_decoration_color = text_decoration;
		}
	}
	else if (property == "text-decoration-line")
	{
		style.text_decoration_line = value;
	}
	else if (property == "text-decoration-color")
	{
		style.text_decoration_color = value[0];
	}
	else if (property == "text-decoration-style")
	{
		style.text_decoration_style = value[0];
	}

  // Border
  else if (property == "border")
  {

  }
  else if (property == "border-width")
  {
    for (int i = 0; i < value.size(); ++i)
    {
      std::string border_width_value = value[i];
      if (is_value_css_number(value[i]))
      {
        CSSNumber number = parse_number(value[i]);
        border_width_value = std::to_string(get_css_number(number, style.font_size));
      }

      if (i == 0)
        style.border_width = {border_width_value, border_width_value, border_width_value, border_width_value};
      else if (i == 1)
      {
        style.border_width[1] = border_width_value; // right
        style.border_width[3] = border_width_value; // left
      }
      else if (i == 2)
        style.border_width[2] = border_width_value; // bottom
      else if (i == 3)
        style.border_width[3] = border_width_value; // left
    }
  }
  else if (property == "border-style")
  {
    for (int i = 0; i < value.size(); ++i)
    {
      std::string border_style_value = value[i];
      if (i == 0)
        style.border_style = {border_style_value, border_style_value, border_style_value, border_style_value};
      else if (i == 1)
      {
        style.border_style[1] = border_style_value; // right
        style.border_style[3] = border_style_value; // left
      }
      else if (i == 2)
        style.border_style[2] = border_style_value; // bottom
      else if (i == 3)
        style.border_style[3] = border_style_value; // left
    }
  }
  else if (property == "border-color")
  {
    for (int i = 0; i < value.size(); ++i)
    {
      std::string border_color_value = value[i];
      if (i == 0)
        style.border_color = {border_color_value, border_color_value, border_color_value, border_color_value};
      else if (i == 1)
      {
        style.border_color[1] = border_color_value; // right
        style.border_color[3] = border_color_value; // left
      }
      else if (i == 2)
        style.border_color[2] = border_color_value; // bottom
      else if (i == 3)
        style.border_color[3] = border_color_value; // left
    }
  }

	// "Cascade-down" property through children if it's inherited
	if (is_property_inherited(property))
	{
		for (HTMLElement* child : children)
		{
			child->set_style_property(property, value);
		}
	}
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
