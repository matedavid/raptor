#include "parser.h"

// === TAG things ===
Tag::Tag()
		: m_type(UNKNOWN_TOKEN), m_value("")
{
}

Tag::Tag(TOKEN_TYPE type, bool is_closing)
		: m_type(type), is_closing(is_closing)
{
}

Tag::Tag(TOKEN_TYPE type, const std::string& value)
		: m_type(type), m_value(value)
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

std::pair<bool, std::string> Tag::attribute(TOKEN_TYPE attr_type) const
{
	auto it = m_attributes.find(attr_type);
	if (it != m_attributes.end())
		return *it;
	return std::make_pair<bool, std::string>(false, "");
}

// ====================

// === Parser things ===
Parser::Parser(Tokenizer& tokenizer)
			 : m_tokenizer(tokenizer)
{
}

Tag Parser::parse_tag(Tokenizer& tokenizer)
{
	tokenizer.next(); // Jump '<' token

	bool is_closing = false;;
	if (tokenizer.current().type == SLASH_TOKEN)
	{
		is_closing = true;
		tokenizer.next();
	}

	Tag tag = Tag(tokenizer.current().type, is_closing);

	tokenizer.next();
	while (tokenizer.current().type != GREATER_THAN_TOKEN)
	{
		TOKEN_TYPE attr_type = tokenizer.current().type;
		tokenizer.next();
		assert(tokenizer.current().type == EQUAL_TOKEN);

		tokenizer.next();
		assert(tokenizer.current().type == TEXT_TOKEN);
		tag.add_atribute(attr_type, tokenizer.current().value);

		tokenizer.next();
	}
	tokenizer.next(); // Jump '>' token

	if (tokenizer.current().type == TEXT_TOKEN)
	{
		tag.set_value(tokenizer.current().value);
		tokenizer.next();
	}

	return tag;
}

void Parser::convert_into_tags()
{
	while (m_tokenizer.current().type != EOF_TOKEN)
	{
		Tag tag = parse_tag(m_tokenizer);

		std::cout << type_as_string(tag.m_type);
		if (tag.is_closing)
			std::cout << " (closing)";
		if (tag.attribute(ID_TOKEN).first)
			std::cout << " [Attributes: " << tag.attribute(ID_TOKEN).second << "]";

		std::cout << " " << tag.m_value << std::endl;

		m_tags.push_back(tag);
	}
}

void Parser::parse()
{
	convert_into_tags();
}
