#pragma once

#include <iostream>
#include <vector>

#include "liquid/html/html_html_element.h"
#include "liquid/html/html_head_element.h"
#include "liquid/html/html_body_element.h"

class HTMLDocument
{
public:
	HTMLHtmlElement* html;

	HTMLHeadElement* head;
	HTMLBodyElement* body;

public:
	HTMLDocument();
};
