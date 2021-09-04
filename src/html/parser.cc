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

void Parser::convert_tokens_into_tags()
{
	while (m_tokenizer.current().type != EOF_TOKEN)
	{
		Tag tag = parse_tag(m_tokenizer);
		m_tags.push_back(tag);
	}
}

Tag Parser::parse_tags_rec(const std::vector<Tag>& tags, int& i)
{
	Tag tag = tags[i];
	if (tag.type() == tags[i+1].type() and tags[i+1].is_closing())
	{
		++i;
		return tag;
	}

	while (tag.type() != tags[i+1].type() or not tags[i+1].is_closing())
	{
		Tag child_tag = parse_tags_rec(tags, ++i);
		tag.add_child(child_tag);
	}

	++i; // Jumps exit tag
	return tag;
}

void Parser::parse()
{
	convert_tokens_into_tags();

	int i = 0;
	m_ast = parse_tags_rec(m_tags, i);
}

const Tag Parser::get_AST() const
{
	return m_ast;
}
