#pragma once

#include "core.h"

#include <vector>

namespace liquid::html {

enum class TokenType
{
  DOCTYPE,
  StartTag,
  EndTag,
  Comment,
  Character,
  EndOfFile
};

using TokenAttribute = std::pair<std::string, std::string>;

class Token
{
  virtual TokenType type() = 0;
};

class DOCTYPEToken : public Token
{
public:
  TokenType type() override { return TokenType::DOCTYPE; }

  // FIXME: Should somehow mark name, public and system identifiers as "missing"
  std::string name;
  std::string public_identifier;
  std::string system_identifier;
  bool force_quirks = false;
};

class StartTagToken : public Token
{
public:
  TokenType type() override { return TokenType::StartTag; }

  std::string tag_name;
  bool self_closing = false;
  std::vector<TokenAttribute> attributes;
};

class EndTagToken : public Token
{
public:
  TokenType type() override { return TokenType::EndTag; }

  std::string tag_name;
  bool self_closing = false;
  std::vector<TokenAttribute> attributes;
};

class CommentToken : public Token
{
public:
  TokenType type() override { return TokenType::Comment; }
  std::string data;
};

class CharacterToken : public Token
{
public:
  TokenType type() override { return TokenType::Character; }
  std::string data;
};

class EOFToken : public Token
{
public:
  TokenType type() override { return TokenType::EndOfFile; }
};

} // namespace liquid::html