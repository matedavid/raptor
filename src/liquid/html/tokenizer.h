#pragma once

#include "core.h"
#include "html/token.h"

#include <stack>

// https://html.spec.whatwg.org/multipage/parsing.html#tokenization
namespace liquid::html {

enum class TokenizerState {
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
    ScriptDataDoubleEscapeEnd,
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
    Comment,
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

class Tokenizer {
  public:
    Tokenizer(const std::string& content);

  private:
    TokenizerState m_current_state;
    TokenizerState m_return_state;
    std::vector<Ref<Token>> m_tokens;
    Ref<Token> m_current_token;
    std::string m_temporary_buffer;

    std::string m_content;
    int m_current_position;

    inline char current_char();
    void consume_next_input_char();
    void reconsume();

    char peek(int jump);
    void consume_jump(int jump);
    bool next_few_characters_are(const std::string& data, bool case_insensitive = false);

    void append_char_current_tag_token_tag_name(char16_t c);
    void append_char_current_comment_token_data(char16_t c);
    void append_char_current_doctype_token_name(char16_t c);
    void append_char_current_doctype_token_public_identifier(char16_t c);
    void append_char_current_doctype_token_system_identifier(char16_t c);
    void set_self_closing_flag_current_token();

    void set_doctype_force_quirks_flag();
    void set_doctype_public_identifier_not_missing();
    void set_doctype_system_identifier_not_missing();

    bool appropriate_end_tag_token(Ref<Token>& token);

    void create_new_attribute(const std::string& name = "", const std::string& value = "");
    void append_char_to_attribute_name(char16_t c);
    void append_char_to_attribute_value(char16_t c);

    void check_attribute_name_not_duplicated();

    void emit_current_token();
    void emit_current_input_char();
    void emit_character_token(char16_t c);
    void emit_DOCTYPE_token();
    void emit_eof_token();

#pragma region TokenizationStates

    void data_state();
    void RCDATA_state();
    void RAWTEXT_state();
    void script_data_state();
    void PLAINTEXT_state();

    void tag_open_state();
    void end_tag_open_state();
    void tag_name_state();

    void RCDATA_less_than_sign_state();
    void RCDATA_end_tag_open_state();
    void RCDATA_end_tag_name_state();
    void RAWTEXT_less_than_sign_state();
    void RAWTEXT_end_tag_open_state();
    void RAWTEXT_end_tag_name_state();

    void script_data_less_than_sign_state();
    void script_data_end_tag_open_state();
    void script_data_end_tag_name_state();
    void script_data_escape_start_state();
    void script_data_escape_start_dash_state();
    void script_data_escaped_state();
    void script_data_escaped_dash_state();
    void script_data_escaped_dash_dash_state();
    void script_data_escaped_less_than_sign_state();
    void script_data_escaped_end_tag_open_state();
    void script_data_escaped_end_tag_name_state();
    void script_data_double_escape_start_state();
    void script_data_double_escaped_state();
    void script_data_double_escaped_dash_state();
    void script_data_double_escaped_dash_dash_state();
    void script_data_double_escaped_less_than_sign_state();
    void script_data_double_escape_end_state();

    void before_attribute_name_state();
    void attribute_name_state();
    void after_attribute_name_state();
    void before_attribute_value_state();
    void attribute_value_double_quoted_state();
    void attribute_value_single_quoted_state();
    void attribute_value_unquoted_state();
    void after_attribute_value_quoted_state();

    void self_closing_start_tag_state();

    void bogus_comment_state();

    void markup_declaration_open_state();

    void comment_start_state();
    void comment_start_dash_state();
    void comment_state();
    void comment_less_than_sign_state();
    void comment_less_than_sign_bang_state();
    void comment_less_than_sign_bang_dash_state();
    void comment_less_than_sign_bang_dash_dash_state();
    void comment_end_dash_state();
    void comment_end_state();
    void comment_end_bang_state();

    void DOCTYPE_state();
    void before_DOCTYPE_name_state();
    void DOCTYPE_name_state();
    void after_DOCTYPE_name_state();
    void after_DOCTYPE_public_keyword_state();
    void before_DOCTYPE_public_identifier_state();
    void DOCTYPE_public_identifier_double_quoted_state();
    void DOCTYPE_public_identifier_single_quoted_state();
    void after_DOCTYPE_public_identifier_state();
    void between_DOCTYPE_public_and_system_identifiers_state();
    void after_DOCTYPE_system_keyword_state();
    void before_DOCTYPE_system_identifier_state();
    void DOCTYPE_system_identifier_double_quoted_state();
    void DOCTYPE_system_identifier_single_quoted_state();
    void after_DOCTYPE_system_identifier_state();
    void bogus_DOCTYPE_state();

    // TODO: Still missing states...

#pragma endregion
};

} // namespace liquid::html