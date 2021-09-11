#pragma once

#include <iostream>

class HTMLElement 
{
public:
	// Metadata attributes
	std::string title;
	std::string lang;
	bool translate;
	std::string dir;

	// User interaction
	bool hidden;
	void click();

	std::string inner_text;
	std::string outer_text;

public:
	HTMLElement();
};
