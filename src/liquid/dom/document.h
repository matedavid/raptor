#pragma once

#include <iostream>

#include "liquid/html/html_element.h"

class HTMLCollection;
class NodeList;

class Document
{
public:
	// DOM Tree accessors
	std::string name;
	std::string title;
	std::string dir;

	HTMLElement body;
	HTMLElement head;

	HTMLCollection images;
	HTMLCollection embeds;
	HTMLCollection plugins;
	HTMLCollection links;
	HTMLCollection forms;
	HTMLCollection scripts;

public:
	Document();

	NodeList get_elements_by_name(std::string element_name);
};
