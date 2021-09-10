#pragma once

#include <iostream>
#include <vector>
#include <utility>

typedef std::pair<std::string, std::string> Attribute;

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

	std::vector<Attribute> attributes;

	std::string public_identifier;
};

std::string token_type_as_string(TokenType type);
