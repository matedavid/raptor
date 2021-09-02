#pragma once

#include <iostream>
#include <vector>

#include "token.h"

class Tokenizer
{
private:
	std::vector<Token> m_tokens;
	int m_position = 0;

	std::string m_content;

private:
	void tokenize_text();
	void tokenize_string();
	void tokenize_special_character();

public:
	Tokenizer();

	void tokenize(const std::string& content);

	const Token current() const;
	void next();
	void prev();
	const Token peek_next() const;
	const Token peek_prev() const;
};
