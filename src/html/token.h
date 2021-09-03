#pragma once

#include <iostream>
#include <string>

enum TOKEN_TYPE
{
	// Common Characters
	LESS_THAN_TOKEN,    // <
	GREATER_THAN_TOKEN, // >
	SLASH_TOKEN,        // /
	EQUAL_TOKEN,        // =

	// Base HTML tags
	HTML_TOKEN,
	HEAD_TOKEN,
	BODY_TOKEN,

	// HTML tags
	TITLE_TOKEN,
	DIV_TOKEN,
	P_TOKEN,
	H1_TOKEN,
	H2_TOKEN,
	H3_TOKEN,
	H4_TOKEN,
	H5_TOKEN,
	H6_TOKEN,
	A_TOKEN,
	IMG_TOKEN,

	// Tag Attributes
	CLASS_TOKEN,
	ID_TOKEN,
	STYLE_TOKEN,

	// OTHER
	TEXT_TOKEN,
	EOF_TOKEN,
	UNKNOWN_TOKEN
};


struct Token
{
	TOKEN_TYPE type;
	std::string value;
};

std::string type_as_string(TOKEN_TYPE type);

TOKEN_TYPE TOKEN_TYPE_from_string(const std::string& str);

TOKEN_TYPE TOKEN_TYPE_from_special_character(const char c);
