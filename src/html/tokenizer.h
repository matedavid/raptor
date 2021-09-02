#pragma once

#include <iostream>
#include <vector>

#include "token.h"

class Tokenizer
{

private:
	std::vector<Token> m_tokens;
	int m_position;

	std::string m_content;

	void tokenize_tag();
	void tokenize_text();

public:
	Tokenizer();

	void tokenize(const std::string& content);

	void next();
	void prev();
	const Token& current() const;

	const Token& peek_next() const;
	const Token& peek_prev() const;
};
