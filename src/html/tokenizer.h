#pragma once

#include <iostream>
#include <vector>

class Tokenizer
{
private:
  // Reference: https://www.w3.org/TR/2011/WD-html5-20110113/tokenization.html#tokenization
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

    CommentStart,
    CommentStartDash,
    Comment,
    CommentEndDash,
    CommentEndState,

    DOCTYPE,
    BeforeDoctypeName,
    DOCTYPEName,
    AfterDOCTYPEName,

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
  };
  
  // The current state of the tokenization process
  State current_state = State::Data;

  std::string m_content;

public:
  Tokenizer();

  void tokenize(const std::string& content);
};