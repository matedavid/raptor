#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include "token.h"

class Tokenizer
{
private:
	// Reference: https://html.spec.whatwg.org/multipage/parsing.html#tokenization
	//            https://www.w3.org/TR/2011/WD-html5-20110113/tokenization.html#tokenization
  enum State
  {
    Data,
    TagOpen,
    EndTagOpen,
    TagName,

    BeforeAttributeName,
    AttributeName,
    AfterAttributeName,

    BeforeAttributeValue,
    DoubleQuotedAttributeValue,
    SingleQuotedAttributeValue,
    UnquotedAttributeValue,
    AfterQuotedAttributeValue,

    SelfClosingStartTag,
		MarkupDeclarationOpenState,

    CommentStart,
    CommentStartDash,
    Comment,
    CommentEndDash,
    CommentEnd,

    DOCTYPE,
    BeforeDOCTYPEName,
    DOCTYPEName,
    AfterDOCTYPEName,

		/*
    AfterDOCTYPEPublicKeyword,
    BeforeDOCTYPEPublicIdentifier,
    DoubleQuotedDOCTYPEPublicIdentifier,
    SingleQuotedDOCTYPEPublicIdentifier,
    AfterDOCTYPEPublicIdentifier,

    BetweenDOCTYPEPublicAndIdentifier,
    AfterDOCTYPESystemKeyword,

    BeforeDOCTYPESystemIdentifier,
    DoubleQuotedDOCTYPESystemIdentifier,
    SingleQuotedDOCTYPESystemIdentifier,
    AfterDOCTYPESystemIdentifier
		*/
  };
  
	// Tokens
	std::vector<Token> m_tokens;

	// Current Tokenizing token
	Token current_token;

  // The current state of the tokenization process
  State current_state = State::Data;

  // Content
  std::string m_content;

  // Position of the tokenizer
  int m_position;

private:
	void consume_data_state();
	void consume_tag_open_state();
	void consume_end_tag_open_state();
	void consume_tag_name_state();
	void consume_before_attribute_name_state();
	void consume_attribute_name_state();
	void consume_after_attribute_name_state();
	void consume_before_attribute_value_state();
	void consume_double_quoted_attribute_value_state();
	void consume_single_quoted_attribute_value_state();
	void consume_unquoted_attribute_value_state();
	void consume_after_quoted_attribute_value_state();

	void consume_self_closing_start_tag_state();
	void consume_markup_declaration_open_state();

  void consume_comment_start_state();
  void consume_comment_start_dash_state();
  void consume_comment_state();
  void consume_comment_end_dash_state();
  void consume_comment_end_state();

  void consume_DOCTYPE_state();
  void consume_before_DOCTYPE_name_state();
  void consume_DOCTYPE_name_state();
  void consume_after_DOCTYPE_name_state();

	// Consumes next input character
	char consume();
	// Returns to previous input character
	void reconsume();

	// Consumes n_chars forward, without modifying the m_position variable
	std::string peek_consume_forward(int n_chars);
	// Consumes n_chars backwards, without modifying the m_position variable
	std::string peek_consume_backwards(int n_chars);

public:
  Tokenizer();

  void tokenize(const std::string& content);

	// Returns the current Token
	Token current() const;
	// Advances forward on the token list
	void next();
	// Advances backwards on the token list
	void prev();
  // Returns if the current position of the tokenizer is the last position
  bool is_last() const;
};
