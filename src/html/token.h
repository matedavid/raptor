#pragma once

#include <iostream>

enum Token_Type
{
	TEXT_TOKEN,

	// Base tags
	HTML_TAG,
	HEAD_TAG,
	BODY_TAG,

	// HEAD tags
	TITLE_TAG,

	// Text tags
	P_TAG,
	H1_TAG,
	H2_TAG,
	H3_TAG,
	H4_TAG,
	H5_TAG,
	H6_TAG,

	EOF_TOKEN,
	UNKNOWN_TAG,
};

class Token
{
private:
	enum Token_Type m_type;
	std::string m_value;

	bool m_closing_tag;

public:
	Token();
	Token(enum Token_Type type, std::string value, bool closing_tag);

	enum Token_Type type() const;
	std::string value() const;
	bool is_closing_tag() const;

	std::string type_as_string() const;
};
