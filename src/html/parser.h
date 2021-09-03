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

private:
	static Tag parse_tag(Tokenizer& tokenizer);

	void convert_into_tags();
	static void parse_tags_rec(const std::vector<Tag>& tags);

public:
	Parser(Tokenizer& tokeznizer);

	void parse();
};
