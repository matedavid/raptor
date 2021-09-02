#include "token.h"

Token::Token()
{
}

Token::Token(enum Token_Type type, std::string value, bool closing_tag)
			: m_type(type), m_value(value), m_closing_tag(closing_tag)
{
}

enum Token_Type Token::type() const
{
	return m_type;
}

std::string Token::value() const
{
	return m_value;
}

bool Token::is_closing_tag() const
{
	return m_closing_tag;
}

std::string Token::type_as_string() const
{
	switch (m_type)
	{
		case TEXT_TOKEN:
			return "TEXT_TOKEN";
		case HTML_TAG:
			return "HTML_TAG";
		case HEAD_TAG:
			return "HEAD_TAG";
		case BODY_TAG:
			return "BODY_TAG";
		case TITLE_TAG:
			return "TITLE_TAG";
		case P_TAG:
			return "P_TAG";
		case H1_TAG:
			return "H1_TAG";
		case H2_TAG:
			return "H2_TAG";
		case H3_TAG:
			return "H3_TAG";
		case H4_TAG:
			return "H4_TAG";
		case H5_TAG:
			return "H5_TAG";
		case H6_TAG:
			return "H6_TAG";
		case EOF_TOKEN:
			return "EOF_TOKEN";
		case UNKNOWN_TAG:
			return "UNKNOWN_TAG";
	}
	return "NOT_FOUND";
}
