#pragma once

#include <iostream>
#include <vector>
#include <utility>

namespace liquid {

typedef std::pair<std::string, std::string> Attr;

enum TokenType
{
  DOCTYPE,
  StartTag,
  EndTag,
  Comment,
  Character,
	EOFToken
};

struct Token
{
	TokenType type;
	std::string value;
	bool self_closing = false;

	std::vector<Attr> attributes;
};

std::string token_type_as_string(TokenType type);

}
