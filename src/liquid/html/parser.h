#pragma once

#include <iostream>
#include <stack>

#include "liquid/html/tokenizer.h"
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

  // The Document being created
  //Document document;

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
