#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "tokenizer.h"


struct AST
{
	Token token;
	std::vector<AST> children;
};

class Parser
{
private:
	Tokenizer m_tokenizer;
	AST m_AST;

	static AST parse_rec(Tokenizer& tokenizer);
	static void print_AST_rec(const AST& ast, int n);

public:
	Parser(Tokenizer& tokenizer);
	void parse();
	void print_AST() const;
};
