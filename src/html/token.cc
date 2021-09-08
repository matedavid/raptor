#include "token.h"

std::string token_type_as_string(TokenType type)
{
	switch (type)
	{
		case TokenType::DOCTYPE:
			return "DOCTYPE";
		case TokenType::StartTag:
			return "StartTag";
		case TokenType::EndTag:
			return "EndTag";
		case TokenType::Comment:
			return "Comment";
		case TokenType::Character:
			return "Character";
		case TokenType::EOFToken:
			return "EOFToken";
		default:
			return "NOT KNOWN";
	}
}
