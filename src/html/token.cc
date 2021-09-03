#include "token.h"

std::string type_as_string(TOKEN_TYPE type)
{
	switch (type)
	{
		case LESS_THAN_TOKEN: return "LESS_THAN_TOKEN";
		case GREATER_THAN_TOKEN: return "GREATER_THAN_TOKEN";
		case SLASH_TOKEN: return "SLASH_TOKEN";
		case EQUAL_TOKEN: return "EQUAL_TOKEN";

		case HTML_TOKEN: return "HTML_TOKEN";
		case HEAD_TOKEN: return "HEAD_TOKEN";
		case BODY_TOKEN: return "BODY_TOKEN";

		case TITLE_TOKEN: return "TITLE_TOKEN";
		case DIV_TOKEN: return "DIV_TOKEN";
		case P_TOKEN: return "P_TOKEN";
		case H1_TOKEN: return "H1_TOKEN";
		case H2_TOKEN: return "H2_TOKEN";
		case H3_TOKEN: return "H3_TOKEN";
		case H4_TOKEN: return "H4_TOKEN";
		case H5_TOKEN: return "H5_TOKEN";
		case H6_TOKEN: return "H6_TOKEN";
		case A_TOKEN: return "A_TOKEN";
		case IMG_TOKEN: return "IMG_TOKEN";

		case CLASS_TOKEN: return "CLASS_TOKEN";
		case ID_TOKEN: return "ID_TOKEN";
		case STYLE_TOKEN: return "STYLE_TOKEN";

		case TEXT_TOKEN: return "TEXT_TOKEN";
		case EOF_TOKEN: return "EOF_TOKEN";
		case UNKNOWN_TOKEN: return "UKNOWN_TOKEN";
	}
	return "NOT_FOUND";
}

TOKEN_TYPE TOKEN_TYPE_from_string(const std::string& str)
{
	if (str == "html") return HTML_TOKEN;
	else if (str == "head") return HEAD_TOKEN;
	else if (str == "body") return BODY_TOKEN;
	else if (str == "title") return TITLE_TOKEN;
	else if (str == "div") return DIV_TOKEN;
	else if (str == "p") return P_TOKEN;
	else if (str == "h1") return H1_TOKEN;
	else if (str == "h2") return H2_TOKEN;
	else if (str == "h3") return H3_TOKEN;
	else if (str == "h4") return H4_TOKEN;
	else if (str == "h5") return H5_TOKEN;
	else if (str == "h6") return H6_TOKEN;
	else if (str == "a") return A_TOKEN;
	else if (str == "img") return IMG_TOKEN;
	else if (str == "class") return CLASS_TOKEN;
	else if (str == "id") return ID_TOKEN;
	else if (str == "style") return STYLE_TOKEN;
	return TEXT_TOKEN;
}

TOKEN_TYPE TOKEN_TYPE_from_special_character(const char c)
{
	switch (c)
	{
		case '<': return LESS_THAN_TOKEN;
		case '>': return GREATER_THAN_TOKEN;
		case '/': return SLASH_TOKEN;
		case '=': return EQUAL_TOKEN;
	}
	return UNKNOWN_TOKEN;
}
