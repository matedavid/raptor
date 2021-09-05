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

	// Metadata
	TITLE_TOKEN,
	STYLE_TOKEN,

	// Content
	DIV_TOKEN,
	HEADER_TOKEN,
	P_TOKEN,
	H1_TOKEN,
	H2_TOKEN,
	H3_TOKEN,
	H4_TOKEN,
	H5_TOKEN,
	H6_TOKEN,
	UL_TOKEN,
	OL_TOKEN,
	LI_TOKEN,
	A_TOKEN,
	STRONG_TOKEN,
	B_TOKEN,
	EM_TOKEN,
	SPAN_TOKEN,
	BR_TOKEN,

	// Media
	IMG_TOKEN,

	// Forms
	BUTTON_TOKEN,
	FORM_TOKEN,
	INPUT_TOKEN,
	LABEL_TOKEN,
	TEXTAREA_TOKEN,

	// Tag Attributes
	CLASS_TOKEN,
	ID_TOKEN,
	SRC_TOKEN,

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
