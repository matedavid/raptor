#include "parser.h"

Parser::Parser(Tokenizer& tokenizer)
			 : m_tokenizer(tokenizer)
{
}

/*
 * If tag 'T' encountered:
 *	advance tokenizer;
 *	call function recursively;
 *	save tag to 'T' children;
 * until equal 'T' closing tag encountered
 *
 * if TEXT_TOKEN encountered
 *	return Text Token;
 */
AST Parser::parse_rec(Tokenizer& tokenizer)
{
	const Token& token = tokenizer.current();
	if (token.type() == TEXT_TOKEN)
		return AST{token};

	AST ast = AST{token};

	tokenizer.next();
	while (tokenizer.current().type() != token.type() or !tokenizer.current().is_closing_tag())
	{
		AST child_ast = parse_rec(tokenizer);
		ast.children.push_back(child_ast);
		tokenizer.next();
	}
	return ast;
}

void Parser::print_AST_rec(const AST& ast, int n)
{
	for (int j = 0; j < n; ++j) std::cout << "\t";
	if (ast.token.type() == TEXT_TOKEN)
		std::cout << ast.token.type_as_string() << std::endl;
	else
	{
		std::cout << ast.token.type_as_string() << std::endl;
		for (int i = 0; i < ast.children.size(); ++i)
			print_AST_rec(ast.children[i], n+1);
	}
}

void Parser::parse()
{
	m_AST = parse_rec(m_tokenizer);
}

void Parser::print_AST() const
{
	print_AST_rec(m_AST, 0);
}
