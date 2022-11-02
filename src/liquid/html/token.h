#pragma once

#include "core.h"

#include <iostream>
#include <utility>
#include <vector>

namespace liquid::html {

enum class TokenType { DOCTYPE, StartTag, EndTag, Comment, Character, EndOfFile };

struct TokenAttribute {
    std::string name;
    std::string value;
};

class Token {
  public:
    virtual TokenType type() = 0;
    virtual void print() = 0;
};

class DOCTYPEToken : public Token {
  public:
    DOCTYPEToken() {}

    DOCTYPEToken(std::string name,
                 std::string public_identifier,
                 std::string system_identifier,
                 bool force_quirks)
        : name(std::move(name)),
          public_identifier(std::move(public_identifier)),
          system_identifier(std::move(system_identifier)),
          force_quirks(force_quirks) {}

    TokenType type() override { return TokenType::DOCTYPE; }

    std::string name;
    std::string public_identifier;
    std::string system_identifier;
    bool force_quirks = false;

    bool name_missing = true;
    bool public_identifier_missing = true;
    bool system_identifier_missing = true;

    void print() override {
        std::cout << "[DOCTYPE]: " << name << " public=" << public_identifier << " system=" << system_identifier
                  << " force_quirks=" << force_quirks << std::endl;
    }
};

class StartTagToken : public Token {
  public:
    StartTagToken() {}

    TokenType type() override { return TokenType::StartTag; }

    std::string tag_name;
    bool self_closing = false;
    std::vector<TokenAttribute> attributes;

    void print() override {
        std::cout << "[StartTag";
        for (const auto& attr : attributes) {
            std::cout << " " << attr.name << "=" << attr.value;
        }

        std::cout << "]: " << tag_name << " " << self_closing << std::endl;
    }
};

class EndTagToken : public Token {
  public:
    EndTagToken() {}

    TokenType type() override { return TokenType::EndTag; }

    std::string tag_name;
    bool self_closing = false;
    std::vector<TokenAttribute> attributes;

    void print() override {
        std::cout << "[EndTag";
        for (const auto& attr : attributes) {
            std::cout << " " << attr.name << "=" << attr.value;
        }

        std::cout << "]: " << tag_name << " " << self_closing << std::endl;
    }
};

class CommentToken : public Token {
  public:
    CommentToken() = default;
    CommentToken(std::string data) : data(std::move(data)) {}

    TokenType type() override { return TokenType::Comment; }
    std::string data;

    void print() override { std::cout << "[Comment]: " << data << std::endl; }
};

class CharacterToken : public Token {
  public:
    CharacterToken(std::string data) : data(std::move(data)) {}

    TokenType type() override { return TokenType::Character; }
    std::string data;

    void print() override { std::cout << "[Character]: " << data << std::endl; }
};

class EOFToken : public Token {
  public:
    TokenType type() override { return TokenType::EndOfFile; }

    void print() override { std::cout << "[EOF]" << std::endl; }
};

} // namespace liquid::html