#pragma once

#include <iostream>

#include "liquid/html/html_element.h"

namespace liquid {

class Text : public HTMLElement
{
private:
	std::string m_content;
	bool trimmed = false;

	// trim from start (in place)
	static void ltrim(std::string &s);
	// trim from end (in place)
	static void rtrim(std::string &s);
	// trim from both ends (in place)
	static void trim(std::string &s);

public:
	Text();
	Text(const std::string& content);

	HTMLElementType type() const override { return HTMLElementType::TextType; }

	std::string content();
	void append_string(const std::string& str);
};

}
