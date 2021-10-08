#pragma once

#include <iostream>

#include "liquid/html/html_element.h"

namespace liquid {

class Text : public HTMLElement
{
private:
	std::string m_content;

public:
	Text();
	Text(const std::string& content);

	HTMLElementType type() override { return HTMLElementType::TextType; }

	std::string content() const;
	void append_string(const std::string& str);
};

}
