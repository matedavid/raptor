#pragma once

#include "core.h"
#include "html/token.h"

#include <stack>

// https://html.spec.whatwg.org/multipage/parsing.html#tokenization
namespace liquid::html {

enum class TokenizerState
{
  Data,
  RCDATA,
  RAWTEXT,
  ScriptData,
  PlainText,
  TagOpen,
  EndTagOpen,
  TagName,
  RCDATALessThanSign,
  RCDATAEndTagOpen,
  RCDATAEndTagName,
  RAWTEXTLessThanSign,
  RAWTEXTEndTagOpen,
  RAWTEXTEndTagName,
  ScriptDataLessThanSign,
  ScriptDataEndTagOpen,
  ScriptDataEndTagName,
  ScriptDataEscapeStart,
  ScriptDataEscapeStartDash,
  ScriptDataEscaped,
  ScriptDataEscapedDash,
  ScriptDataEscapedDashDash,
  ScriptDataEscapedLessThanSign,
  ScriptDataEscapedEndTagOpen,
  ScriptDataEscapedEndTagName,
  ScriptDataDoubleEscapeStart,
  ScriptDataDoubleEscaped,
  ScriptDataDoubleEscapedDash,
  ScriptDataDoubleEscapedDashDash,
  ScriptDataDoubleEscapedLessThanSign,
  ScriptDataDoubleEscapedEnd,
  BeforeAttributeName,
  AttributeName,
  AfterAttributeName,
  BeforeAttributeValue,
  AttributeValueDoubleQuoted,
  AttributeValueSingleQuoted,
  AttributeValueUnquoted,
  AfterAttributeValueQuoted,
  SelfClosingStartTag,
  BogusComment,
  MarkupDeclarationOpen,
  CommentStart,
  CommentStartDash,
  CommentState,
  CommentLessThanSign,
  CommentLessThanSignBang,
  CommentLessThanSignBangDash,
  CommentLessThanSignBangDashDash,
  CommentEndDash,
  CommentEnd,
  CommentEndBang,
  DOCTYPE,
  BeforeDOCTYPEName,
  DOCTYPEName,
  AfterDOCTYPEName,
  AfterDOCTYPEPublicKeyword,
  BeforeDOCTYPEPublicIdentifier,
  DOCTYPEPublicIdentifierDoubleQuoted,
  DOCTYPEPublicIdentifierSingleQuoted,
  AfterDOCTYPEPublicIdentifier,
  BetweenDOCTYPEPublicAndSystemIdentifiers,
  AfterDOCTYPESystemKeyword,
  BeforeDOCTYPESystemIdentifier,
  DOCTYPESystemIdentifierDoubleQuoted,
  DOCTYPESystemIdentifierSingleQuoted,
  AfterDOCTYPESystemIdentifier,
  BogusDOCTYPE,
  CDATASection,
  CDATASectionBracket,
  CDATASectionEnd,
  CharacterReference,
  NamedCharacterReference,
  AmbiguousAmpersand,
  NumericCharacterReference,
  HexadecimalCharacterReferenceStart,
  DecimalCharacterReferenceStart,
  HexadecimalCharacterReference,
  DecimalCharacterReference,
  NumericCharacterReferenceEnd
};

class Tokenizer
{
public:
  Tokenizer(const std::string& content);

private:
  TokenizerState m_current_state;
  std::stack<Token> open_elements;

  std::string m_content;
  int m_current_position;

  char consume();
  char reconsume();
};

} // namespace liquid::html