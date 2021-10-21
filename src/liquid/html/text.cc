#include "text.h"

namespace liquid {

void Text::ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

void Text::rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

void Text::trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

Text::Text()
{
  element_value = "text";
}

Text::Text(const std::string& content)
	: m_content(content)
{
  element_value = "text";
}

std::string Text::content() 
{
	if (not trimmed)
	{
		trim(m_content);
		trimmed = true;
	}

	return m_content;
}

void Text::append_string(const std::string& str)
{
	m_content += str;
	trimmed = false;
}

}
