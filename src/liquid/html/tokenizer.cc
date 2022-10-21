#include "tokenizer.h"

#include <iostream>

namespace liquid::html {

Tokenizer::Tokenizer(const std::string& content)
{
  m_content = content;
  current_position = -1;

  current_state = TokenizerState::Data;

  while (current_position + 1 < m_content.length()) {
    switch (current_state) {
      case TokenizerState::Data:
        data_state();
        break;
      case TokenizerState::RCDATA:
        RCDATA_state();
        break;
      case TokenizerState::RAWTEXT:
        RAWTEXT_state();
        break;
      case TokenizerState::ScriptData:
        script_data_state();
        break;
      case TokenizerState::PlainText:
        PLAINTEXT_state();
        break;
      case TokenizerState::TagOpen:
        tag_open_state();
        break;
      case TokenizerState::EndTagOpen:
        end_tag_open_state();
        break;
      default:
        current_position++;
        break;
    }
  }

  std::cout << "Created tokens: " << tokens.size() << std::endl;
}

char Tokenizer::consume_next_input_char()
{
  if (current_position++ >= m_content.size())
    current_position++;

  return m_content[current_position];
}

char Tokenizer::reconsume()
{
  if (current_position-- < 0)
    current_position++;

  return m_content[current_position];
}

void Tokenizer::emit_DOCTYPE_token() {}

void Tokenizer::emit_start_tag_token() {}

void Tokenizer::emit_end_tag_token() {}

void Tokenizer::emit_comment_token(const std::string& data)
{
  auto token = std::make_shared<CommentToken>(data);
  tokens.push_back(token);
}

void Tokenizer::emit_character_token(char16_t c)
{
  std::string content = std::to_string(c);
  auto token = std::make_shared<CharacterToken>(content);
  tokens.push_back(token);
}

void Tokenizer::emit_eof_token()
{
  auto token = std::make_shared<EOFToken>();
  tokens.push_back(token);
}

void Tokenizer::data_state()
{
  char c = consume_next_input_char();

  if (c == '&') {
    return_state = TokenizerState::Data;
    current_state = TokenizerState::CharacterReference;
  } else if (c == '<') {
    current_state = TokenizerState::TagOpen;
  } else if (c == 0) {
    emit_character_token(c);
    // TODO: Throw UnexpectedNullCharacterParseError
  } else if (c == EOF) {
    // TODO: emit_eof_token();
  } else {
    emit_character_token(c);
  }
}

void Tokenizer::RCDATA_state()
{
  char c = consume_next_input_char();

  if (c == '&') {
    return_state = TokenizerState::RCDATA;
    current_state = TokenizerState::CharacterReference;
  } else if (c == '<') {
    current_state = TokenizerState::RCDATALessThanSign;
  } else if (c == 0) {
    // TODO: Throw UnexpectedNullCharacterParseError
    emit_character_token(u'\uFFFD'); // Replacement character
  } else if (c == EOF) {
    emit_eof_token();
  } else {
    emit_character_token(c);
  }
}

void Tokenizer::RAWTEXT_state()
{
  char c = consume_next_input_char();

  if (c == '<') {
    current_state = TokenizerState::RAWTEXTLessThanSign;
  } else if (c == 0) {
    // TODO: Throw UnexpectedNullCharacterParseError
    emit_character_token(u'\uFFFD'); // Replacement character
  } else if (c == EOF) {
    emit_eof_token();
  } else {
    emit_character_token(c);
  }
}

void Tokenizer::script_data_state()
{
  char c = consume_next_input_char();

  if (c == '<') {
    current_state = TokenizerState::ScriptDataLessThanSign;
  } else if (c == 0) {
    // TODO: UnexpectedNullCharacterParseError
    emit_character_token(u'\uFFFD'); // Replacement character
  } else if (c == EOF) {
    emit_eof_token();
  } else {
    emit_character_token(c);
  }
}

void Tokenizer::PLAINTEXT_state()
{
  char c = consume_next_input_char();

  if (c == 0) {
    // TODO: UnexpectedNullCharacterParseError
    emit_character_token(u'\uFFFD'); // Replacement character
  } else if (c == EOF) {
    emit_eof_token();
  } else {
    emit_character_token(c);
  }
}

void Tokenizer::tag_open_state()
{
  char c = consume_next_input_char();

  if (c == '!') {
    current_state = TokenizerState::MarkupDeclarationOpen;
  } else if (c == '/') {
    current_state = TokenizerState::EndTagOpen;
  } else if (std::isalpha(c)) {
    auto token = std::make_shared<StartTagToken>();
    open_elements.push(token);
    reconsume();
    current_state = TokenizerState::TagName;
  } else if (c == '?') {
    // TODO: Throw UnexpectedQuestionMarkInsteadOfTagNameParseError
    emit_comment_token("");
    reconsume();
    current_state = TokenizerState::BogusComment;
  } else if (c == EOF) {
    // TODO: Throw EOFBeforeTagNameParseError
    emit_character_token('<');
    emit_eof_token();
  } else {
    // TODO: Throw InvalidFirstCharacterOfTagNameParseError
    emit_character_token('<');
    reconsume();
    current_state = TokenizerState::Data;
  }
}

void Tokenizer::end_tag_open_state()
{
  char c = consume_next_input_char();

  if (std::isalpha(c)) {
    auto token = std::make_shared<EndTagToken>();
    open_elements.push(token);
    reconsume();
    current_state = TokenizerState::TagName;
  } else if (c == '>') {
    // TODO: Throw MissingEndTagNameParseError
    current_state = TokenizerState::Data;
  } else if (c == EOF) {
    // TODO: Throw EOFBeforeTagNameParseError
    emit_character_token('<');
    emit_character_token('/');
    emit_eof_token();
  } else {
    // TODO: Throw InvalidFirstCharacterOfTagNameParseError
    auto token = std::make_shared<CommentToken>("");
    open_elements.push(token);
    reconsume();
    current_state = TokenizerState::BogusComment;
  }
}

} // namespace liquid::html