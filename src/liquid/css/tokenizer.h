#pragma once

#include <iostream>
#include <vector>
#include <cctype>

#include "liquid/css/token.h"

namespace liquid {

class CSSTokenizer
{
private:
  enum State
  {
    BeforeSelector,
    InSelector,
    InBlock,
    InProperty,
    InValue,
    InComment
  };

	// Tokens
	std::vector<CSSToken> m_tokens;
  
  // String content to tokenize
  std::string m_content;

	// Position of the tokenizer
	int m_position;

  // Current state of the tokenizer
  State m_current_state;

  // The current token being created
  CSSToken m_current_token;

  // True if in_value is inside a parenthesis 
  bool inside_parenthesis = false;

  // Previous state if the Tokenizer needs to remember (eg. entering comment and then continue where it left of)
  State m_previous_state;
  // Previous Token if the Tokenizer needs to remember (eg. entering comment and then continue where it left of)
  CSSToken m_previous_token;
  
  // Returns the nearest not-yet consumed token
  char consume();
  // Reconsumes the current token
  void reconsume();

  // Consumes n_chars forward, without modifying the m_position variable
  std::string peek_consume_forward(int n_chars);
  // Consumes n_chars backwards, without modifying the m_position variable
  std::string peek_consume_backwards(int n_chars);
  // Returns the next character that is not a space
  char next_non_white_character();

  void consume_before_selector();
  void consume_in_selector();
  void consume_in_block();
  void consume_in_property();
  void consume_in_value();
  void consume_in_comment();

public:
	CSSTokenizer();

	void tokenize(const std::string& content);
  
  CSSToken current() const;
  CSSToken next();
  CSSToken prev();
  bool is_last() const;
};

}
