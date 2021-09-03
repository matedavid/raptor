#include "tag.h"

Tag::Tag()
		: m_type(UNKNOWN_TOKEN), m_value("")
{
}

Tag::Tag(TOKEN_TYPE type, bool is_closing)
		: m_type(type), m_closing(is_closing)
{
}

Tag::Tag(TOKEN_TYPE type, const std::string& value, bool is_closing)
		: m_type(type), m_value(value), m_closing(is_closing)
{
}

void Tag::set_value(const std::string& value)
{
	m_value = value;
}

void Tag::add_child(Tag child)
{
	m_children.push_back(child);
}

void Tag::add_atribute(TOKEN_TYPE attr_type, const std::string& attr_value)
{
	m_attributes[attr_type] = attr_value;
}

std::vector<Tag> Tag::innerHTML() const
{
	return m_children;
}

std::string Tag::text() const
{
	return m_value;
}

TOKEN_TYPE Tag::type() const
{
	return m_type;
}

bool Tag::is_closing() const
{
	return m_closing;
}

std::pair<bool, std::string> Tag::attribute(TOKEN_TYPE attr_type) const
{
	auto it = m_attributes.find(attr_type);
	if (it != m_attributes.end())
		return *it;
	return std::make_pair<bool, std::string>(false, "");
}
