#include "tag.h"

void Tag::find_tags_rec(const Tag& ast, const TOKEN_TYPE type, std::vector<Tag>& tags)
{
	if (ast.type() == type)
		tags.push_back(ast);

	if (not ast.text().empty())
		return;

	for (int i = 0; i < ast.m_children.size(); ++i)
		find_tags_rec(ast.m_children[i], type, tags);
}

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

std::vector<Tag> Tag::innerHTML() const
{
	return m_children;
}

std::vector<Tag> Tag::tag(const TOKEN_TYPE type) const
{
	if (type == m_type)
		return std::vector<Tag>{*this};

	std::vector<Tag> results;
	find_tags_rec(*this, type, results);
	return results;
}

std::pair<bool, std::string> Tag::attribute(TOKEN_TYPE attr_type) const
{
	auto it = m_attributes.find(attr_type);
	if (it != m_attributes.end())
		return *it;
	return std::make_pair<bool, std::string>(false, "");
}

std::map<TOKEN_TYPE, std::string> Tag::attributes() const
{
	return m_attributes;
}

std::ostream& operator<<(std::ostream& os, const Tag& tag)
{
	os << "[" << type_as_string(tag.type());
	if (tag.attributes().size() != 0)
	{
		os << " ";
		auto attrs = tag.attributes();

		auto it = attrs.begin();
		while (it != attrs.end())
		{
			os << "(" << type_as_string(it->first) << "=" << it->second << ")";
			++it;
		}
	}
	os << "]";
	if (not tag.text().empty())
		os << ": " << tag.text();
	return os;
}
