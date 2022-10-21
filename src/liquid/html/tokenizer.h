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
  TokenizerState current_state;
  TokenizerState return_state;
  std::vector<Ref<Token>> tokens;
  std::stack<Ref<Token>> open_elements;

  std::string m_content;
  int current_position;

  char consume_next_input_char();
  char reconsume();

  void emit_DOCTYPE_token();
  void emit_start_tag_token();
  void emit_end_tag_token();
  void emit_comment_token(const std::string& data);
  void emit_character_token(char16_t c);
  void emit_eof_token();

  void data_state();
  void RCDATA_state();
  void RAWTEXT_state();
  void script_data_state();
  void PLAINTEXT_state();
  void tag_open_state();
  void end_tag_open_state();
};

} // namespace liquid::html