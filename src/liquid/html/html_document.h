#pragma once

#include <iostream>
#include <filesystem>

#include "liquid/html/tokenizer.h"
#include "liquid/html/parser.h"
#include "liquid/html/html_html_element.h"
#include "liquid/html/html_head_element.h"
#include "liquid/html/html_body_element.h"

namespace liquid {

class HTMLDocument
{
public:
	HTMLHtmlElement* html;

	HTMLHeadElement* head;
	HTMLBodyElement* body;

	std::string title;

public:
	HTMLDocument();

	void from_string(const std::string& content);
	void from_file(const std::filesystem::path& file_path);
};

}
