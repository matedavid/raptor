#pragma once

#include <iostream>

#include "liquid/html/html_element.h"

class Text : public HTMLElement
{
private:
	std::string m_content;

public:
	Text();
	Text(const std::string& content);

	std::string content() const;
	void append_string(const std::string& str);
};

