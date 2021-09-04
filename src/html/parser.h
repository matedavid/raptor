#pragma once

#include <iostream>
#include <vector>
#include <assert.h>

#include "tokenizer.h"
#include "tag.h"

class Parser
{
private:
	Tokenizer& m_tokenizer;
	std::vector<Tag> m_tags;

	Tag m_ast;

private:
	static Tag parse_tag(Tokenizer& tokenizer);

	void convert_tokens_into_tags();
	static Tag parse_tags_rec(const std::vector<Tag>& tags, int& i);
	void print_tags_recursive(const Tag& tag, int spaces);

public:
	Parser(Tokenizer& tokeznizer);

	void parse();
};
