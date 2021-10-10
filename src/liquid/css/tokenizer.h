#pragma once

#include <iostream>
#include <vector>
#include <cctype>

#include "liquid/css/token.h"

namespace liquid {

class CSSTokenizer
{
private:
	// Tokens
	std::vector<CSSToken> m_tokens;

	// The current state of the Tokenizer	
	State current_state;
  
  // String content to tokenize
  std::string m_content;

	// Position of the tokenizer
	int m_position;
  
  // Returns the nearest not-yet consumed token
  char consume();
  // Reconsumes the current token
  void reconsume();

  // Consumes n_chars forward, without modifying the m_position variable
  std::string peek_consume_forward(int n_chars);
  // Consumes n_chars backwards, without modifying the m_position variable
  std::string peek_consume_backwards(int n_chars);

  /* Helper tokenization functions */
  bool is_name_start_code_point(const char c);
  bool is_name_code_point(const char c);


  /* Tokenization algorithms */
  CSSToken consume_token();
  CSSToken consume_comment();

  bool are_valid_escape(const std::string& str);
  bool would_start_identifier(const std::string& str);
  bool would_start_number(const std::string& str);

  std::string consume_name();
  void consume_number();
  void convert_string_to_number();
  void consume_remnants_bad_url();

public:
	CSSTokenizer();

	void tokenize();
  
  CSSToken current() const;
  void next();
  void prev();
};

}
