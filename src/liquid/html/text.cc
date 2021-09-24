#include "text.h"

Text::Text()
{
}

Text::Text(const std::string& content)
	: m_content(content)
{
}

std::string Text::content() const
{
	return m_content;
}

void Text::append_string(const std::string& str)
{
	m_content += str;
}


