#pragma once

#include <iostream>
#include <stack>

#include "liquid/html/debug.h"

#include "liquid/html/tokenizer.h"
#include "liquid/html/text.h"

#include "liquid/html/html_element.h"
#include "liquid/html/html_html_element.h"
#include "liquid/html/html_head_element.h"
#include "liquid/html/html_body_element.h"
#include "liquid/html/html_title_element.h"

namespace liquid {

class Parser
{
private:
  enum InsertionMode
  {
    TextMode,
    Initial,
    BeforeHTML,
    BeforeHead,
    InHead,
    InHeadNoscript,
    AfterHead,
    InBody,
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
  
  // The insertion mode to which, after processing, the parser will return
  InsertionMode original_insertion_mode;

  // Object resulting of a previous tokenization
  Tokenizer m_tokenizer;

	// Indicates if the parser should reconsume the last consumed token
	bool reconsume_token;

	// The document being constructed
	HTMLHtmlElement* html;

	// Stack of elements that are currently open
	std::stack<HTMLElement*> open_elements;

	// Inserts a character under the element at the top of open elements
	void insert_character(const Token& token, HTMLElement* element);

  /* Functions to resolve InsertionModes */
  void text_mode();
	void initial_mode();
  void before_html_mode();
  void before_head_mode();
  void in_head_mode();
  //void in_head_no_script_mode();
  void after_head_mode();
  void in_body_mode();
	void after_body_mode();

public:
  Parser();

	HTMLHtmlElement* parse(Tokenizer& tokenizer);
};

}
