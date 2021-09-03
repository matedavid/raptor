#include "parser.h"

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
		m_tags.push_back(tag);
	}
}

void Parser::parse_tags_rec(const std::vector<Tag>& tags)
{
}

void Parser::parse()
{
	convert_into_tags();
	for (int i = 0; i < m_tags.size(); ++i)
	{
		Tag tag = m_tags[i];
		std::cout << type_as_string(tag.type());
		if (tag.is_closing())
			std::cout << " (closing)";
		if (tag.attribute(ID_TOKEN).first)
			std::cout << " [Attributes: " << tag.attribute(ID_TOKEN).second << "]";

		std::cout << " " << tag.text() << std::endl;
	}

	parse_tags_rec(m_tags);
}
