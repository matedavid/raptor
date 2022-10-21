#pragma once

#include "core.h"

#include <utility>
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
  DOCTYPEToken() {}

  DOCTYPEToken(std::string name,
               std::string public_identifier,
               std::string system_identifier,
               bool force_quirks)
      : name(std::move(name)),
        public_identifier(std::move(public_identifier)),
        system_identifier(std::move(system_identifier)),
        force_quirks(force_quirks)
  {
  }

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
  StartTagToken() {}

  TokenType type() override { return TokenType::StartTag; }

  std::string tag_name;
  bool self_closing = false;
  std::vector<TokenAttribute> attributes;
};

class EndTagToken : public Token
{
public:
  EndTagToken() {}

  TokenType type() override { return TokenType::EndTag; }

  std::string tag_name;
  bool self_closing = false;
  std::vector<TokenAttribute> attributes;
};

class CommentToken : public Token
{
public:
  CommentToken(std::string data) : data(std::move(data)) {}

  TokenType type() override { return TokenType::Comment; }
  std::string data;
};

class CharacterToken : public Token
{
public:
  CharacterToken(std::string data) : data(std::move(data)) {}

  TokenType type() override { return TokenType::Character; }
  std::string data;
};

class EOFToken : public Token
{
public:
  TokenType type() override { return TokenType::EndOfFile; }
};

} // namespace liquid::html