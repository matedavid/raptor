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
		case STYLE_TOKEN: return "STYLE_TOKEN";

		case DIV_TOKEN: return "DIV_TOKEN";
		case HEADER_TOKEN: return "HEADER_TOKEN";
		case P_TOKEN: return "P_TOKEN";
		case H1_TOKEN: return "H1_TOKEN";
		case H2_TOKEN: return "H2_TOKEN";
		case H3_TOKEN: return "H3_TOKEN";
		case H4_TOKEN: return "H4_TOKEN";
		case H5_TOKEN: return "H5_TOKEN";
		case H6_TOKEN: return "H6_TOKEN";
		case UL_TOKEN: return "UL_TOKEN";
		case OL_TOKEN: return "OL_TOKEN";
		case LI_TOKEN: return "LI_TOKEN";
		case A_TOKEN: return "A_TOKEN";
		case STRONG_TOKEN: return "STRONG_TOKEN";
		case B_TOKEN: return "B_TOKEN";
		case EM_TOKEN: return "EM_TOKEN";
		case SPAN_TOKEN: return "SPAN_TOKEN";
		case BR_TOKEN: return "BR_TOKEN";

		case IMG_TOKEN: return "IMG_TOKEN";

		case BUTTON_TOKEN: return "BUTTON_TOKEN";
		case FORM_TOKEN: return "FORM_TOKEN";
		case INPUT_TOKEN: return "INPUT_TOKEN";
		case LABEL_TOKEN: return "LABEL_TOKEN";
		case TEXTAREA_TOKEN: return "TEXTAREA_TOKEN";


		case CLASS_TOKEN: return "CLASS_TOKEN";
		case ID_TOKEN: return "ID_TOKEN";
		case SRC_TOKEN: return "SRC_TOKEN";

		case TEXT_TOKEN: return "TEXT_TOKEN";
		case EOF_TOKEN: return "EOF_TOKEN";
		case UNKNOWN_TOKEN: return "UKNOWN_TOKEN";
	}
	return "NOT_FOUND";
}

TOKEN_TYPE TOKEN_TYPE_from_string(const std::string& str)
{
	// Base tokens
	if (str == "html") return HTML_TOKEN;
	else if (str == "head") return HEAD_TOKEN;
	else if (str == "body") return BODY_TOKEN;

	// Metedata
	else if (str == "title") return TITLE_TOKEN;
	else if (str == "style") return STYLE_TOKEN;

	// Content
	else if (str == "div") return DIV_TOKEN;
	else if (str == "header") return HEADER_TOKEN;
	else if (str == "p") return P_TOKEN;
	else if (str == "h1") return H1_TOKEN;
	else if (str == "h2") return H2_TOKEN;
	else if (str == "h3") return H3_TOKEN;
	else if (str == "h4") return H4_TOKEN;
	else if (str == "h5") return H5_TOKEN;
	else if (str == "h6") return H6_TOKEN;
	else if (str == "ul") return UL_TOKEN;
	else if (str == "ol") return OL_TOKEN;
	else if (str == "li") return LI_TOKEN;
	else if (str == "a") return A_TOKEN;
	else if (str == "strong") return STRONG_TOKEN;
	else if (str == "b") return B_TOKEN;
	else if (str == "em") return EM_TOKEN;
	else if (str == "span") return SPAN_TOKEN;
	else if (str == "br") return BR_TOKEN;

	// Media
	else if (str == "img") return IMG_TOKEN;

	// Forms
	else if (str == "button") return BUTTON_TOKEN;
	else if (str == "form") return FORM_TOKEN;
	else if (str == "input") return INPUT_TOKEN;
	else if (str == "label") return LABEL_TOKEN;
	else if (str == "textarea") return TEXTAREA_TOKEN;

	// Attributes
	else if (str == "class") return CLASS_TOKEN;
	else if (str == "id") return ID_TOKEN;
	else if (str == "src") return SRC_TOKEN;

	// If no specified token was found, it's a text token
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
