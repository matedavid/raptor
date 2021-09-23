#pragma once

#include <iostream>
#include <stack>

#include "liquid/html/tokenizer.h"
#include "liquid/html/html_document.h"
#include "liquid/html/html_element.h"

class Parser
{
private:
  enum InsertionMode
  {
    Initial,
    BeforeHTML,
    BeforeHead,
    InHead,
    InHeadNoscript,
    AfterHead,
    InBody,
    Text,
    InTable,
    InTableText,
    InCaption,
    InColumnGroup,
    InTableBody,
    InRow,
    InCell,
    InSelect,
    InSelectInTable,
    InTemplate,
    AfterBody,
    InFrameset,
    AfterFrameset,
    AfterAfterBody,
    AfterAfterFrameset
  };

  // The current insertion mode of the parser
  InsertionMode current_insertion_mode;

  // Object resulting of a previous tokenization
  Tokenizer m_tokenizer;

	// Indicates if the parser should reconsume the last consumed token
	bool reconsume_token;

	// Pointer to the last referenced element
	HTMLElement* current_element;

  std::stack<HTMLElement> open_elements;

  /* Functions to resolve InsertionModes */
  void initial_mode();
  void before_html_mode();
  void before_head_mode();
  void in_head_mode();
  void in_head_no_script_mode();
  void after_head_mode();
  void in_body_mode();
  void text_mode();

public:
  Parser();

  void parse(Tokenizer& tokenizer);
};
