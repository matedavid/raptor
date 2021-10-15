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
    InValue
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

public:
	CSSTokenizer();

	void tokenize(const std::string& content);
  
  CSSToken current() const;
  void next();
  void prev();
};

}
