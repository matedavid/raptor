#pragma once

#include <iostream>
#include <filesystem>
#include <stack>

#include "liquid/html/debug.h"

#include "liquid/html/tokenizer.h"
#include "liquid/html/text.h"

#include "liquid/html/html_element.h"
#include "liquid/html/html_html_element.h"
#include "liquid/html/html_head_element.h"
#include "liquid/html/html_body_element.h"
#include "liquid/html/html_div_element.h"
#include "liquid/html/html_title_element.h"
#include "liquid/html/html_paragraph_element.h"
#include "liquid/html/html_span_element.h"
#include "liquid/html/html_heading_element.h"
#include "liquid/html/html_emphasis_element.h"
#include "liquid/html/html_italicized_element.h"
#include "liquid/html/html_strong_element.h"
#include "liquid/html/html_anchor_element.h"
#include "liquid/html/html_ordered_list_element.h"
#include "liquid/html/html_unordered_list_element.h"
#include "liquid/html/html_list_item_element.h"
#include "liquid/html/html_image_element.h"


namespace liquid {

class HTMLParser
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
    InStyle,
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

  // Path to the html document being parsed, used for relative file references
  std::filesystem::path m_document_path;

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
  void in_style_mode();
  //void in_head_no_script_mode();
  void after_head_mode();
  void in_body_mode();
	void after_body_mode();

public:
	HTMLParser();

	HTMLHtmlElement* parse(Tokenizer& tokenizer, const std::string& document_path);
};

}
