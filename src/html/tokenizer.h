#pragma once

#include <iostream>
#include <vector>

#include "token.h"

class Tokenizer
{

private:

	std::string m_content;
	int m_position;

	void tokenize_tag();
	void tokenize_text();

public:
	std::vector<Token> m_tokens;
	Tokenizer();

	void tokenize(const std::string& content);

	void next();
	void prev();

	void peek_next();
	void peek_prev();
};
