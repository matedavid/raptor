#include "text.h"

namespace liquid {

Text::Text()
{
  element_value = "text";
}

Text::Text(const std::string& content)
	: m_content(content)
{
  element_value = "text";
}

std::string Text::content() const
{
	return m_content;
}

void Text::append_string(const std::string& str)
{
	m_content += str;
}

}
