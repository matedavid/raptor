#include "tokenizer.h"

#include <iostream>

namespace liquid::html {

#pragma region macros

#define SwitchTo(state) m_current_state = TokenizerState::state
#define SetReturnState(state) m_return_state = TokenizerState::state
#define ReconsumeIn(state) \
    reconsume();           \
    SwitchTo(state)

#define Ascii(c) std::isalpha(c)
#define AsciiLower(c) std::isalpha(c) && std::islower(c)
#define AsciiUpper(c) std::isalpha(c) && std::isupper(c)

#define CreateToken(T, ...) m_current_token = MakeRef(T, __VA_ARGS__)
#define GetTokenAs(token, T) std::static_pointer_cast<T>(token)

#define cNULL 0
#define REPLACEMENT_CHARACTER u'\uFFFD'
#define CHARACTER_TABULATION u'\u0009'
#define LINE_FEED u'\u000A'
#define FORM_FEED u'\u000C'
#define SPACE u'\u0020'

#define TOKENIZER_ERROR(state, message) LOG_WARN("[TokenizerParseError]: {} - {}", state, message)

#pragma endregion

Tokenizer::Tokenizer(const std::string& content) {
    m_content = content;
    m_current_position = -1;

    m_current_state = TokenizerState::Data;
    m_return_state = TokenizerState::Data;

    while (m_current_position + 1 < m_content.length()) {
        consume_next_input_char();

        switch (m_current_state) {
            case TokenizerState::Data:
                data_state();
                break;
            case TokenizerState::RCDATA:
                RCDATA_state();
                break;
            case TokenizerState::RAWTEXT:
                RAWTEXT_state();
                break;
            case TokenizerState::ScriptData:
                script_data_state();
                break;
            case TokenizerState::PlainText:
                PLAINTEXT_state();
                break;
            case TokenizerState::TagOpen:
                tag_open_state();
                break;
            case TokenizerState::EndTagOpen:
                end_tag_open_state();
                break;
            case TokenizerState::TagName:
                tag_name_state();
                break;
            case TokenizerState::RCDATALessThanSign:
                RCDATA_less_than_sign_state();
                break;
            case TokenizerState::RCDATAEndTagOpen:
                RCDATA_end_tag_open_state();
                break;
            case TokenizerState::RCDATAEndTagName:
                RCDATA_end_tag_name_state();
                break;
            case TokenizerState::RAWTEXTLessThanSign:
                RAWTEXT_less_than_sign_state();
                break;
            case TokenizerState::RAWTEXTEndTagOpen:
                RAWTEXT_end_tag_open_state();
                break;
            case TokenizerState::RAWTEXTEndTagName:
                RAWTEXT_end_tag_name_state();
                break;
            case TokenizerState::ScriptDataLessThanSign:
                script_data_less_than_sign_state();
                break;
            case TokenizerState::ScriptDataEndTagOpen:
                script_data_end_tag_open_state();
                break;
            case TokenizerState::ScriptDataEndTagName:
                script_data_end_tag_name_state();
                break;
            case TokenizerState::ScriptDataEscapeStart:
                script_data_escape_start_state();
                break;
            case TokenizerState::ScriptDataEscapeStartDash:
                script_data_escape_start_dash_state();
                break;
            case TokenizerState::ScriptDataEscaped:
                script_data_escaped_state();
                break;
            case TokenizerState::ScriptDataEscapedDash:
                script_data_escaped_dash_state();
                break;
            case TokenizerState::ScriptDataEscapedDashDash:
                script_data_escaped_dash_dash_state();
                break;
            case TokenizerState::ScriptDataEscapedLessThanSign:
                script_data_escaped_less_than_sign_state();
                break;
            case TokenizerState::ScriptDataEscapedEndTagOpen:
                script_data_escaped_end_tag_open_state();
                break;
            case TokenizerState::ScriptDataEscapedEndTagName:
                script_data_escaped_end_tag_name_state();
                break;
            case TokenizerState::ScriptDataDoubleEscapeStart:
                script_data_double_escape_start_state();
                break;
            case TokenizerState::ScriptDataDoubleEscaped:
                script_data_double_escaped_state();
                break;
            case TokenizerState::ScriptDataDoubleEscapedDash:
                script_data_double_escaped_dash_state();
                break;
            case TokenizerState::ScriptDataDoubleEscapedDashDash:
                script_data_double_escaped_dash_dash_state();
                break;
            case TokenizerState::ScriptDataDoubleEscapedLessThanSign:
                script_data_double_escaped_less_than_sign_state();
                break;
            case TokenizerState::ScriptDataDoubleEscapeEnd:
                script_data_double_escape_end_state();
                break;
            case TokenizerState::BeforeAttributeName:
                before_attribute_name_state();
                break;
            case TokenizerState::AttributeName:
                attribute_name_state();
                break;
            case TokenizerState::AfterAttributeName:
                after_attribute_name_state();
                break;
            case TokenizerState::BeforeAttributeValue:
                before_attribute_value_state();
                break;
            case TokenizerState::AttributeValueDoubleQuoted:
                attribute_value_double_quoted_state();
                break;
            case TokenizerState::AttributeValueSingleQuoted:
                attribute_value_single_quoted_state();
                break;
            case TokenizerState::AttributeValueUnquoted:
                attribute_value_unquoted_state();
                break;
            case TokenizerState::AfterAttributeValueQuoted:
                after_attribute_value_quoted_state();
                break;
            case TokenizerState::SelfClosingStartTag:
                self_closing_start_tag_state();
                break;
            case TokenizerState::BogusComment:
                bogus_comment_state();
                break;
            case TokenizerState::MarkupDeclarationOpen:
                markup_declaration_open_state();
                break;
            case TokenizerState::CommentStart:
                comment_start_state();
                break;
            case TokenizerState::CommentStartDash:
                comment_start_dash_state();
                break;
            case TokenizerState::Comment:
                comment_state();
                break;
            case TokenizerState::CommentLessThanSign:
                comment_less_than_sign_state();
                break;
            case TokenizerState::CommentLessThanSignBang:
                comment_less_than_sign_bang_state();
                break;
            case TokenizerState::CommentLessThanSignBangDash:
                comment_less_than_sign_bang_dash_state();
                break;
            case TokenizerState::CommentLessThanSignBangDashDash:
                comment_less_than_sign_bang_dash_dash_state();
                break;
            case TokenizerState::CommentEndDash:
                comment_end_dash_state();
                break;
            case TokenizerState::CommentEnd:
                comment_end_state();
                break;
            case TokenizerState::CommentEndBang:
                comment_end_bang_state();
                break;
            case TokenizerState::DOCTYPE:
                DOCTYPE_state();
                break;
            case TokenizerState::BeforeDOCTYPEName:
                before_DOCTYPE_name_state();
                break;
            case TokenizerState::DOCTYPEName:
                DOCTYPE_name_state();
                break;
            case TokenizerState::AfterDOCTYPEName:
                after_DOCTYPE_name_state();
                break;
            case TokenizerState::AfterDOCTYPEPublicKeyword:
                after_DOCTYPE_public_keyword_state();
                break;
            case TokenizerState::BeforeDOCTYPEPublicIdentifier:
                before_DOCTYPE_public_identifier_state();
                break;
            case TokenizerState::DOCTYPEPublicIdentifierDoubleQuoted:
                DOCTYPE_public_identifier_double_quoted_state();
                break;
            case TokenizerState::DOCTYPEPublicIdentifierSingleQuoted:
                DOCTYPE_public_identifier_single_quoted_state();
                break;
            case TokenizerState::AfterDOCTYPEPublicIdentifier:
                after_DOCTYPE_public_identifier_state();
                break;
            case TokenizerState::BetweenDOCTYPEPublicAndSystemIdentifiers:
                between_DOCTYPE_public_and_system_identifiers_state();
                break;
            case TokenizerState::AfterDOCTYPESystemKeyword:
                after_DOCTYPE_system_keyword_state();
                break;
            case TokenizerState::BeforeDOCTYPESystemIdentifier:
                before_DOCTYPE_system_identifier_state();
                break;
            case TokenizerState::DOCTYPESystemIdentifierDoubleQuoted:
                DOCTYPE_system_identifier_double_quoted_state();
                break;
            case TokenizerState::DOCTYPESystemIdentifierSingleQuoted:
                DOCTYPE_system_identifier_single_quoted_state();
                break;
            case TokenizerState::AfterDOCTYPESystemIdentifier:
                after_DOCTYPE_system_identifier_state();
                break;
            case TokenizerState::BogusDOCTYPE:
                bogus_DOCTYPE_state();
                break;
        }
    }

    std::cout << "Created tokens: " << m_tokens.size() << std::endl;
    for (const auto& token : m_tokens) {
        token->print();
    }
}

char Tokenizer::current_char() {
    return m_content[m_current_position];
}

void Tokenizer::consume_next_input_char() {
    if (m_current_position + 1 < m_content.size())
        m_current_position++;
}

void Tokenizer::reconsume() {
    if (m_current_position - 1 >= 0)
        m_current_position--;
}

char Tokenizer::peek(int jump) {
    int jump_position = m_current_position + jump;
    assert(jump_position >= 0 && jump_position < m_content.length());

    return m_content[jump];
}

void Tokenizer::consume_jump(int jump) {
    int jump_position = m_current_position + jump;
    assert(jump_position >= 0 && jump_position < m_content.length());
    // -1 because consume_next_input_char jumps to the next
    m_current_position = jump_position - 1;
}

bool Tokenizer::next_few_characters_are(const std::string& data, bool case_insensitive) {
    int size = data.length();
    assert(m_current_position + size < m_content.length());

    for (int i = 0; i < size; ++i) {
        int content_pos = m_current_position + i;
        char c = case_insensitive ? char(std::tolower(m_content[content_pos])) : data[content_pos];
        if (data[i] != c) {
            return false;
        }
    }
    return true;
}

void Tokenizer::append_char_current_tag_token_tag_name(char16_t c) {
    assert(m_current_token->type() == TokenType::StartTag ||
           m_current_token->type() == TokenType::EndTag);

    if (m_current_token->type() == TokenType::StartTag) {
        auto token = GetTokenAs(m_current_token, StartTagToken);
        token->tag_name += char(c);
    } else if (m_current_token->type() == TokenType::EndTag) {
        auto token = GetTokenAs(m_current_token, EndTagToken);
        token->tag_name += char(c);
    }
}

void Tokenizer::append_char_current_comment_token_data(char16_t c) {
    assert(m_current_token->type() == TokenType::Comment);

    auto token = GetTokenAs(m_current_token, CommentToken);
    token->data += char(c);
}

void Tokenizer::append_char_current_doctype_token_name(char16_t c) {
    assert(m_current_token->type() == TokenType::DOCTYPE);

    auto token = GetTokenAs(m_current_token, DOCTYPEToken);
    token->name += char(c);
}

void Tokenizer::append_char_current_doctype_token_public_identifier(char16_t c) {
    assert(m_current_token->type() == TokenType::DOCTYPE);

    auto token = GetTokenAs(m_current_token, DOCTYPEToken);
    token->public_identifier += char(c);
}

void Tokenizer::append_char_current_doctype_token_system_identifier(char16_t c) {
    assert(m_current_token->type() == TokenType::DOCTYPE);

    auto token = GetTokenAs(m_current_token, DOCTYPEToken);
    token->system_identifier += char(c);
}

void Tokenizer::set_self_closing_flag_current_token() {
    assert(m_current_token->type() == TokenType::StartTag);

    auto token = GetTokenAs(m_current_token, StartTagToken);
    token->self_closing = true;
}

void Tokenizer::set_doctype_force_quirks_flag() {
    assert(m_current_token->type() == TokenType::DOCTYPE);

    auto token = GetTokenAs(m_current_token, DOCTYPEToken);
    token->force_quirks = true;
}

void Tokenizer::set_doctype_public_identifier_not_missing() {
    assert(m_current_token->type() == TokenType::DOCTYPE);

    auto token = GetTokenAs(m_current_token, DOCTYPEToken);
    token->public_identifier_missing = false;
    token->public_identifier = "";
}

void Tokenizer::set_doctype_system_identifier_not_missing() {
    assert(m_current_token->type() == TokenType::DOCTYPE);

    auto token = GetTokenAs(m_current_token, DOCTYPEToken);
    token->system_identifier_missing = false;
    token->system_identifier = "";
}

bool Tokenizer::appropriate_end_tag_token(Ref<Token>& token) {
    if (token->type() != TokenType::EndTag) {
        LOG_ERROR("Token is not EndTag");
        exit(1);
    }

    auto end_tag = std::static_pointer_cast<EndTagToken>(token);

    for (int i = m_tokens.size() - 1; i >= 0; --i) {
        if (m_tokens[i]->type() == TokenType::StartTag) {
            // auto starttag = std::static_pointer_cast<StartTagToken>(m_tokens[i]);
            auto start_tag = GetTokenAs(m_tokens[i], StartTagToken);
            return start_tag->tag_name == end_tag->tag_name;
        }
    }

    return false;
}

void Tokenizer::create_new_attribute(const std::string& name, const std::string& value) {
    assert(m_current_token->type() == TokenType::StartTag);
    auto token = GetTokenAs(m_current_token, StartTagToken);
    token->attributes.push_back(TokenAttribute{.name = name, .value = value});
}

void Tokenizer::append_char_to_attribute_name(char16_t c) {
    assert(m_current_token->type() == TokenType::StartTag);
    auto token = GetTokenAs(m_current_token, StartTagToken);
    token->attributes[token->attributes.size() - 1].name += char(c);
}

void Tokenizer::append_char_to_attribute_value(char16_t c) {
    assert(m_current_token->type() == TokenType::StartTag);
    auto token = GetTokenAs(m_current_token, StartTagToken);
    token->attributes[token->attributes.size() - 1].value += char(c);
}

void Tokenizer::check_attribute_name_not_duplicated() {
    assert(m_current_token->type() == TokenType::StartTag);
    auto token = GetTokenAs(m_current_token, StartTagToken);

    const std::string& attribute_name = token->attributes[token->attributes.size() - 1].name;

    for (int i = token->attributes.size() - 2; i >= 0; --i) {
        if (token->attributes[i].name == attribute_name) {
            TOKENIZER_ERROR("LeavingAttributeName", "Duplicate attribute");
            token->attributes.pop_back();
            break;
        }
    }
}

void Tokenizer::emit_current_token() {
    m_tokens.push_back(m_current_token);
}

void Tokenizer::emit_current_input_char() {
    emit_character_token(current_char());
}

void Tokenizer::emit_character_token(char16_t c) {
    std::string content(1, c);
    auto token = MakeRef(CharacterToken, content);
    m_tokens.push_back(token);
}

void Tokenizer::emit_DOCTYPE_token() {
}

void Tokenizer::emit_eof_token() {
    auto token = MakeRef(EOFToken);
    m_tokens.push_back(token);
}

#pragma region TokenizationStates

void Tokenizer::data_state() {
    char c = current_char();

    if (c == '&') {
        SetReturnState(Data);
        SwitchTo(CharacterReference);
    } else if (c == '<') {
        SwitchTo(TagOpen);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("DATA", "Unexpected null character");
        emit_current_input_char();
    } else if (c == EOF) {
        emit_eof_token();
    } else {
        emit_current_input_char();
    }
}

void Tokenizer::RCDATA_state() {
    char c = current_char();

    if (c == '&') {
        SetReturnState(RCDATALessThanSign);
        SwitchTo(CharacterReference);
    } else if (c == '<') {
        SwitchTo(RCDATALessThanSign);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("RCDATA", "Unexpected null character");
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        emit_eof_token();
    } else {
        emit_current_input_char();
    }
}

void Tokenizer::RAWTEXT_state() {
    char c = current_char();

    if (c == '<') {
        SwitchTo(RAWTEXTLessThanSign);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("RAWTEXT", "Unexpected null character");
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        emit_eof_token();
    } else {
        emit_current_input_char();
    }
}

void Tokenizer::script_data_state() {
    char c = current_char();

    if (c == '<') {
        SwitchTo(ScriptDataLessThanSign);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("ScriptData", "Unexpected null character");
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        emit_eof_token();
    } else {
        emit_current_input_char();
    }
}

void Tokenizer::PLAINTEXT_state() {
    char c = current_char();

    if (c == cNULL) {
        TOKENIZER_ERROR("PLAINTEXT", "Unexpected null character");
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        emit_eof_token();
    } else {
        emit_current_input_char();
    }
}

void Tokenizer::tag_open_state() {
    char c = current_char();

    if (c == '!') {
        SwitchTo(MarkupDeclarationOpen);
    } else if (c == '/') {
        SwitchTo(EndTagOpen);
    } else if (Ascii(c)) {
        CreateToken(StartTagToken);
        ReconsumeIn(TagName);
    } else if (c == '?') {
        TOKENIZER_ERROR("TagOpen", "Unexpected question mark instead of tag name");
        CreateToken(CommentToken);
        ReconsumeIn(BogusComment);
    } else if (c == EOF) {
        TOKENIZER_ERROR("TagOpen", "EOF before tag name");
        emit_character_token('<');
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("TagOpen", "Invalid first character of tag name");
        emit_character_token('<');
        ReconsumeIn(Data);
    }
}

void Tokenizer::end_tag_open_state() {
    char c = current_char();

    if (Ascii(c)) {
        CreateToken(EndTagToken);
        ReconsumeIn(TagName);
    } else if (c == '>') {
        TOKENIZER_ERROR("EndTagOpen", "Missing end tag name");
        SwitchTo(Data);
    } else if (c == EOF) {
        TOKENIZER_ERROR("EndTagOpen", "EOF before tag name");
        emit_character_token('<');
        emit_character_token('/');
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("EndTagOpen", "Invalid first character of tag name");
        CreateToken(CommentToken);
        ReconsumeIn(BogusComment);
    }
}

void Tokenizer::tag_name_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(BeforeAttributeName);
    } else if (c == '/') {
        SwitchTo(SelfClosingStartTag);
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (AsciiUpper(c)) {
        append_char_current_tag_token_tag_name(std::tolower(c));
    } else if (c == cNULL) {
        TOKENIZER_ERROR("TagName", "Unexpected null character");
        append_char_current_tag_token_tag_name(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("TagName", "EOF in tag");
        emit_eof_token();
    } else {
        append_char_current_tag_token_tag_name(c);
    }
}

void Tokenizer::RCDATA_less_than_sign_state() {
    char c = current_char();

    if (c == '/') {
        m_temporary_buffer = "";
        SwitchTo(RCDATAEndTagOpen);
    } else {
        emit_character_token('<');
        ReconsumeIn(RCDATA);
    }
}

void Tokenizer::RCDATA_end_tag_open_state() {
    char c = current_char();

    if (Ascii(c)) {
        CreateToken(EndTagToken);
        ReconsumeIn(RCDATAEndTagName);
    } else {
        emit_character_token('<');
        emit_character_token('/');
        ReconsumeIn(RCDATA);
    }
}

void Tokenizer::RCDATA_end_tag_name_state() {
    char c = current_char();
    bool anything_else = true;

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(BeforeAttributeName);
            anything_else = false;
        }
    } else if (c == '/') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(SelfClosingStartTag);
            anything_else = false;
        }
    } else if (c == '>') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(Data);
            emit_current_token();
            anything_else = false;
        }
    } else if (AsciiUpper(c)) {
        append_char_current_tag_token_tag_name(std::tolower(c));
        m_temporary_buffer += c;
        anything_else = false;
    } else if (AsciiLower(c)) {
        append_char_current_tag_token_tag_name(c);
        m_temporary_buffer += c;
        anything_else = false;
    }

    if (anything_else) {
        emit_character_token('<');
        for (char buff_char : m_temporary_buffer) {
            emit_character_token(buff_char);
        }
        SwitchTo(RCDATA);
    }
}

void Tokenizer::RAWTEXT_less_than_sign_state() {
    char c = current_char();

    if (c == '/') {
        m_temporary_buffer = "";
        SwitchTo(RAWTEXTEndTagOpen);
    } else {
        emit_character_token('<');
        ReconsumeIn(RAWTEXT);
    }
}

void Tokenizer::RAWTEXT_end_tag_open_state() {
    char c = current_char();

    if (Ascii(c)) {
        CreateToken(EndTagToken);
        ReconsumeIn(RAWTEXTEndTagName);
    } else {
        emit_character_token('<');
        emit_character_token('/');
        ReconsumeIn(RAWTEXT);
    }
}

void Tokenizer::RAWTEXT_end_tag_name_state() {
    char c = current_char();
    bool anything_else = true;

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(BeforeAttributeName);
            anything_else = false;
        }
    } else if (c == '/') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(SelfClosingStartTag);
            anything_else = false;
        }
    } else if (c == '>') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(Data);
            emit_current_token();
            anything_else = false;
        }
    } else if (AsciiUpper(c)) {
        append_char_current_tag_token_tag_name(std::tolower(c));
        m_temporary_buffer += c;
        anything_else = false;
    } else if (AsciiLower(c)) {
        append_char_current_tag_token_tag_name(c);
        m_temporary_buffer += c;
        anything_else = false;
    }

    if (anything_else) {
        emit_character_token('<');
        emit_character_token('/');
        for (char buff_char : m_temporary_buffer) {
            emit_character_token(buff_char);
        }
        ReconsumeIn(RAWTEXT);
    }
}

void Tokenizer::script_data_less_than_sign_state() {
    char c = current_char();

    if (c == '/') {
        m_temporary_buffer = "";
        SwitchTo(ScriptDataEndTagOpen);
    } else if (c == '!') {
        SwitchTo(ScriptDataEscapeStart);
        emit_character_token('<');
        emit_character_token('!');
    } else {
        emit_character_token('<');
        ReconsumeIn(ScriptData);
    }
}

void Tokenizer::script_data_end_tag_open_state() {
    char c = current_char();

    if (Ascii(c)) {
        CreateToken(EndTagToken);
        ReconsumeIn(ScriptDataEndTagName);
    } else {
        emit_character_token('<');
        emit_character_token('/');
        ReconsumeIn(ScriptData);
    }
}
void Tokenizer::script_data_end_tag_name_state() {
    char c = current_char();
    bool anything_else = true;

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(BeforeAttributeName);
            anything_else = false;
        }
    } else if (c == '/') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(SelfClosingStartTag);
            anything_else = false;
        }
    } else if (c == '>') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(Data);
            emit_current_token();
            anything_else = false;
        }
    } else if (AsciiUpper(c)) {
        append_char_current_tag_token_tag_name(std::tolower(c));
        m_temporary_buffer += c;
        anything_else = false;
    } else if (AsciiLower(c)) {
        append_char_current_tag_token_tag_name(c);
        m_temporary_buffer += c;
        anything_else = false;
    }

    if (anything_else) {
        emit_character_token('<');
        emit_character_token('/');
        for (char buff_char : m_temporary_buffer) {
            emit_character_token(buff_char);
        }
        ReconsumeIn(ScriptData);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-escape-start-state
void Tokenizer::script_data_escape_start_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(ScriptDataEscapeStartDash);
        emit_character_token('-');
    } else {
        ReconsumeIn(ScriptData);
    }
}

void Tokenizer::script_data_escape_start_dash_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(ScriptDataEscapedDashDash);
        emit_character_token('-');
    } else {
        ReconsumeIn(ScriptData);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-state
void Tokenizer::script_data_escaped_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(ScriptDataEscapedDash);
        emit_character_token('-');
    } else if (c == '<') {
        SwitchTo(ScriptDataEscapedLessThanSign);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("ScriptDataEscaped", "Unexpected null character");
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("ScriptDataEscaped", "EOF in script html comment like text");
        emit_eof_token();
    } else {
        emit_current_input_char();
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-dash-state
void Tokenizer::script_data_escaped_dash_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(ScriptDataEscapedDashDash);
        emit_character_token('-');
    } else if (c == '<') {
        SwitchTo(ScriptDataEscapedLessThanSign);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("ScriptDataEscapedDash", "Unexpected null character");
        SwitchTo(ScriptDataEscaped);
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("ScriptDataEscapedDash", "EOF in script html comment like text");
        emit_eof_token();
    } else {
        SwitchTo(ScriptDataEscaped);
        emit_current_input_char();
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-dash-dash-state
void Tokenizer::script_data_escaped_dash_dash_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(ScriptDataEscapedDashDash);
        emit_character_token('-');
    } else if (c == '<') {
        SwitchTo(ScriptDataEscapedLessThanSign);
    } else if (c == '>') {
        SwitchTo(ScriptData);
        emit_character_token('>');
    } else if (c == cNULL) {
        TOKENIZER_ERROR("ScriptDataEscapedDashDash", "Unexpected null character");
        SwitchTo(ScriptDataEscaped);
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("ScriptDataEscapedDashDash", "EOF in script html comment like text");
        emit_eof_token();
    } else {
        SwitchTo(ScriptDataEscaped);
        emit_current_input_char();
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-less-than-sign-state
void Tokenizer::script_data_escaped_less_than_sign_state() {
    char c = current_char();

    if (c == '/') {
        m_temporary_buffer = "";
        SwitchTo(ScriptDataEscapedEndTagOpen);
    } else if (Ascii(c)) {
        m_temporary_buffer = "";
        emit_character_token('<');
        ReconsumeIn(ScriptDataDoubleEscapeStart);
    } else {
        emit_character_token('<');
        ReconsumeIn(ScriptDataEscaped);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-open-state
void Tokenizer::script_data_escaped_end_tag_open_state() {
    char c = current_char();

    if (Ascii(c)) {
        CreateToken(EndTagToken);
        ReconsumeIn(ScriptDataEscapedEndTagName);
    } else {
        emit_character_token('<');
        emit_character_token('/');
        ReconsumeIn(ScriptDataEscaped);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-escaped-end-tag-name-state
void Tokenizer::script_data_escaped_end_tag_name_state() {
    char c = current_char();
    bool anything_else = true;

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(BeforeAttributeName);
            anything_else = false;
        }
    } else if (c == '/') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(SelfClosingStartTag);
            anything_else = false;
        }
    } else if (c == '>') {
        if (appropriate_end_tag_token(m_current_token)) {
            SwitchTo(Data);
            emit_current_token();
            anything_else = false;
        }
    } else if (AsciiUpper(c)) {
        append_char_current_tag_token_tag_name(std::tolower(c));
        m_temporary_buffer += c;
        anything_else = false;
    } else if (AsciiLower(c)) {
        append_char_current_tag_token_tag_name(c);
        m_temporary_buffer += c;
        anything_else = false;
    }

    if (anything_else) {
        emit_character_token('<');
        emit_character_token('/');
        for (char buff_char : m_temporary_buffer) {
            emit_character_token(buff_char);
        }
        ReconsumeIn(ScriptDataEscaped);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-start-state
void Tokenizer::script_data_double_escape_start_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE || c == '/' ||
        c == '>') {
        if (m_temporary_buffer == "script") {
            SwitchTo(ScriptDataDoubleEscaped);
        } else {
            SwitchTo(ScriptDataEscaped);
            emit_current_input_char();
        }
    } else if (AsciiUpper(c)) {
        m_temporary_buffer += std::tolower(c);
        emit_current_input_char();
    } else if (AsciiLower(c)) {
        m_temporary_buffer += c;
        emit_current_input_char();
    } else {
        ReconsumeIn(ScriptDataEscaped);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-state
void Tokenizer::script_data_double_escaped_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(ScriptDataDoubleEscapedDash);
        emit_character_token('-');
    } else if (c == '<') {
        SwitchTo(ScriptDataDoubleEscapedLessThanSign);
        emit_character_token('<');
    } else if (c == cNULL) {
        TOKENIZER_ERROR("ScriptDataDoubleEscaped", "Unexpected null character");
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("ScriptDataDoubleEscaped", "EOF in script comment like text");
        emit_eof_token();
    } else {
        emit_current_input_char();
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-dash-state
void Tokenizer::script_data_double_escaped_dash_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(ScriptDataDoubleEscapedDashDash);
        emit_character_token('-');
    } else if (c == '<') {
        SwitchTo(ScriptDataDoubleEscapedLessThanSign);
        emit_character_token('<');
    } else if (c == cNULL) {
        TOKENIZER_ERROR("ScriptDataDoubleEscapedDash", "Unexpected null character");
        SwitchTo(ScriptDataDoubleEscaped);
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("ScriptDataDoubleEscapedDash", "EOF in script comment like text");
        emit_eof_token();
    } else {
        SwitchTo(ScriptDataDoubleEscaped);
        emit_current_input_char();
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-dash-dash-state
void Tokenizer::script_data_double_escaped_dash_dash_state() {
    char c = current_char();

    if (c == '-') {
        emit_character_token('-');
    } else if (c == '<') {
        SwitchTo(ScriptDataDoubleEscapedLessThanSign);
        emit_character_token('<');
    } else if (c == '>') {
        SwitchTo(ScriptData);
        emit_character_token('>');
    } else if (c == cNULL) {
        TOKENIZER_ERROR("ScriptDataDoubleEscapedDashDash", "Unexpected null character");
        emit_character_token(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("ScriptDataDoubleEscapedDash", "EOF in script comment like text");
        emit_eof_token();
    } else {
        SwitchTo(ScriptDataDoubleEscaped);
        emit_current_input_char();
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escaped-less-than-sign-state
void Tokenizer::script_data_double_escaped_less_than_sign_state() {
    char c = current_char();

    if (c == '/') {
        m_temporary_buffer = "";
        SwitchTo(ScriptDataDoubleEscapeEnd);
        emit_character_token('/');
    } else {
        ReconsumeIn(ScriptDataDoubleEscaped);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#script-data-double-escape-end-state
void Tokenizer::script_data_double_escape_end_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE || c == '/' ||
        c == '>') {
        if (m_temporary_buffer == "script") {
            SwitchTo(ScriptDataEscaped);
        } else {
            SwitchTo(ScriptDataDoubleEscaped);
            emit_current_input_char();
        }
    } else if (AsciiUpper(c)) {
        m_temporary_buffer += std::tolower(c);
        emit_current_input_char();
    } else if (AsciiLower(c)) {
        m_temporary_buffer += c;
        emit_current_input_char();
    } else {
        ReconsumeIn(ScriptDataDoubleEscaped);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#before-attribute-name-state
void Tokenizer::before_attribute_name_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore character
    } else if (c == '/' || c == '>' || c == EOF) {
        ReconsumeIn(AfterAttributeName);
    } else if (c == '=') {
        TOKENIZER_ERROR("BeforeAttributeName", "Unexpected equals sign before attribute name");
        create_new_attribute(std::to_string(c), "");
        SwitchTo(AttributeName);
    } else {
        create_new_attribute();
        ReconsumeIn(AttributeName);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#attribute-name-state
void Tokenizer::attribute_name_state() {
    char c = current_char();

    if (c == '\t' || c == LINE_FEED || c == FORM_FEED || c == SPACE || c == '/' || c == '>' ||
        c == EOF) {
        ReconsumeIn(AfterAttributeName);
    } else if (c == '=') {
        SwitchTo(BeforeAttributeValue);
    } else if (AsciiUpper(c)) {
        append_char_to_attribute_name(std::tolower(c));
    } else if (c == cNULL) {
        TOKENIZER_ERROR("AttributeNameState", "Unexpected null character");
        append_char_to_attribute_name(REPLACEMENT_CHARACTER);
    } else if (c == '"' || c == '\'' || c == '<') {
        TOKENIZER_ERROR("AttributeNameState", "Unexpected character in attribute name");

        // "Anything else" entry
        append_char_to_attribute_name(c);
    } else {
        append_char_to_attribute_name(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#after-attribute-name-state
void Tokenizer::after_attribute_name_state() {
    char c = current_char();

    // Ignore character
    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore character
    } else if (c == '/') {
        SwitchTo(SelfClosingStartTag);
    } else if (c == '=') {
        SwitchTo(BeforeAttributeValue);
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("AfterAttributeName", "EOF in tag");
        emit_eof_token();
    } else {
        create_new_attribute();
        ReconsumeIn(AttributeName);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#before-attribute-value-state
void Tokenizer::before_attribute_value_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore character
    } else if (c == '"') {
        SwitchTo(AttributeValueDoubleQuoted);
    } else if (c == '\'') {
        SwitchTo(AttributeValueSingleQuoted);
    } else if (c == '>') {
        TOKENIZER_ERROR("BeforeAttributeValue", "Missing attribute value");
        SwitchTo(Data);
        emit_current_token();
    } else {
        ReconsumeIn(AttributeValueUnquoted);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(double-quoted)-state
void Tokenizer::attribute_value_double_quoted_state() {
    char c = current_char();

    if (c == '"') {
        SwitchTo(AfterAttributeValueQuoted);
    } else if (c == '&') {
        SetReturnState(AttributeValueDoubleQuoted);
        SwitchTo(CharacterReference);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("AttributeValueDoubleQuoted", "Unexpected null character");
        append_char_to_attribute_value(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("AttributeValueDoubleQuoted", "EOF in tag");
        emit_eof_token();
    } else {
        append_char_to_attribute_value(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(single-quoted)-state
void Tokenizer::attribute_value_single_quoted_state() {
    char c = current_char();

    if (c == '\'') {
        SwitchTo(AfterAttributeValueQuoted);
    } else if (c == '&') {
        SetReturnState(AttributeValueSingleQuoted);
        SwitchTo(CharacterReference);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("AttributeValueSingleQuoted", "Unexpected null character");
        append_char_to_attribute_value(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("AttributeValueSingleQuoted", "EOF in tag");
        emit_eof_token();
    } else {
        append_char_to_attribute_value(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#attribute-value-(unquoted)-state
void Tokenizer::attribute_value_unquoted_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(BeforeAttributeName);
        check_attribute_name_not_duplicated();
    } else if (c == '&') {
        SetReturnState(AttributeValueUnquoted);
        SwitchTo(CharacterReference);
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
        check_attribute_name_not_duplicated();
    } else if (c == cNULL) {
        TOKENIZER_ERROR("AttributeValueUnquoted", "Unexpected null character");
        append_char_to_attribute_value(REPLACEMENT_CHARACTER);
    } else if (c == '"' || c == '\'' || c == '<' || c == '=' || c == '`') {
        TOKENIZER_ERROR("AttributeValueUnquoted",
                        "Unexpected character in unquoted attribute value");

        // Anything else entry
        append_char_to_attribute_value(c);
    } else if (c == EOF) {
        TOKENIZER_ERROR("AttributeValueUnquoted", "EOF in tag");
        emit_eof_token();
    } else {
        append_char_to_attribute_value(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#after-attribute-value-(quoted)-state
void Tokenizer::after_attribute_value_quoted_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(BeforeAttributeName);
    } else if (c == '/') {
        SwitchTo(SelfClosingStartTag);
    } else if (c == '>') {
        SwitchTo(Data);
    } else if (c == EOF) {
        TOKENIZER_ERROR("AttributeValueQuoted", "EOF in tag");
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("AttributeValueQuoted", "Missing whitespace between attributes");
        ReconsumeIn(BeforeAttributeName);
    }

    check_attribute_name_not_duplicated();
}

// https://html.spec.whatwg.org/multipage/parsing.html#self-closing-start-tag-state
void Tokenizer::self_closing_start_tag_state() {
    char c = current_char();

    if (c == '>') {
        set_self_closing_flag_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("SelfClosingStartTag", "EOF in tag");
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("SelfClosingStartTag", "Unexpected solidus in tag");
        ReconsumeIn(BeforeAttributeName);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#bogus-comment-state
void Tokenizer::bogus_comment_state() {
    char c = current_char();

    if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        emit_current_token();
        emit_eof_token();
    } else if (c == cNULL) {
        TOKENIZER_ERROR("BogusComment", "Unexpected null character");
        append_char_current_comment_token_data(REPLACEMENT_CHARACTER);
    } else {
        append_char_current_comment_token_data(c);
    }
}

// TODO: https://html.spec.whatwg.org/multipage/parsing.html#markup-declaration-open-state
void Tokenizer::markup_declaration_open_state() {
    if (next_few_characters_are("--")) {
        consume_jump(2);
        CreateToken(CommentToken);
        SwitchTo(CommentStart);
    } else if (next_few_characters_are("doctype", true)) {
        consume_jump(7);
        SwitchTo(DOCTYPE);
    } else {
        LOG_WARN("This branch of Markup declaration not implemented");
        assert(false);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-start-state
void Tokenizer::comment_start_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(CommentStartDash);
    } else if (c == '>') {
        TOKENIZER_ERROR("CommentStart", "Abrupt closing of empty comment");
        SwitchTo(Data);
        emit_current_token();
    } else {
        ReconsumeIn(Comment);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-start-dash-state
void Tokenizer::comment_start_dash_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(CommentEnd);
    } else if (c == '>') {
        TOKENIZER_ERROR("CommentStartDash", "Abrupt closing of empty comment");
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("CommentStartDash", "EOF in comment");
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_comment_token_data('-');
        ReconsumeIn(Comment);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-state
void Tokenizer::comment_state() {
    char c = current_char();

    if (c == '<') {
        append_char_current_comment_token_data(c);
    } else if (c == '-') {
        SwitchTo(CommentEndDash);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("Comment", "Unexpected null character");
        append_char_current_comment_token_data(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("Comment", "EOF in comment");
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_comment_token_data(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-state
void Tokenizer::comment_less_than_sign_state() {
    char c = current_char();

    if (c == '!') {
        append_char_current_comment_token_data(c);
        SwitchTo(CommentLessThanSignBang);
    } else if (c == '<') {
        append_char_current_comment_token_data(c);
    } else {
        ReconsumeIn(Comment);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-state
void Tokenizer::comment_less_than_sign_bang_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(CommentLessThanSignBangDash);
    } else {
        ReconsumeIn(Comment);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-dash-state
void Tokenizer::comment_less_than_sign_bang_dash_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(CommentLessThanSignBangDashDash);
    } else {
        ReconsumeIn(CommentEndDash);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-less-than-sign-bang-dash-dash-state
void Tokenizer::comment_less_than_sign_bang_dash_dash_state() {
    char c = current_char();

    if (c == '>' || c == EOF) {
        ReconsumeIn(CommentEnd);
    } else {
        TOKENIZER_ERROR("CommentLessThanSignBangDashDash", "Nested component");
        ReconsumeIn(CommentEnd);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-end-dash-state
void Tokenizer::comment_end_dash_state() {
    char c = current_char();

    if (c == '-') {
        SwitchTo(CommentEnd);
    } else if (c == EOF) {
        TOKENIZER_ERROR("CommentEndDash", "EOF in comment");
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_comment_token_data('-');
        ReconsumeIn(Comment);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-end-state
void Tokenizer::comment_end_state() {
    char c = current_char();

    if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (c == '!') {
        SwitchTo(CommentEndBang);
    } else if (c == '-') {
        append_char_current_comment_token_data('-');
    } else if (c == EOF) {
        TOKENIZER_ERROR("CommentEnd", "EOF in comment");
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_comment_token_data('-');
        append_char_current_comment_token_data('-');
        ReconsumeIn(Comment);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#comment-end-bang-state
void Tokenizer::comment_end_bang_state() {
    char c = current_char();

    if (c == '-') {
        append_char_current_comment_token_data('-');
        append_char_current_comment_token_data('-');
        append_char_current_comment_token_data('!');
        SwitchTo(CommentEndDash);
    } else if (c == '>') {
        TOKENIZER_ERROR("CommentEndBang", "Incorrectly closed comment");
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("CommentEndBang", "EOF in comment");
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_comment_token_data('-');
        append_char_current_comment_token_data('-');
        append_char_current_comment_token_data('!');
        ReconsumeIn(Comment);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#doctype-state
void Tokenizer::DOCTYPE_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(BeforeDOCTYPEName);
    } else if (c == '>') {
        ReconsumeIn(BeforeDOCTYPEName);
    } else if (c == EOF) {
        TOKENIZER_ERROR("DOCTYPEState", "EOF in DOCTYPE");
        CreateToken(DOCTYPEToken);
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("DOCTYPEState", "Missing whitespace before DOCTYPE name");
        ReconsumeIn(BeforeDOCTYPEName);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-name-state
void Tokenizer::before_DOCTYPE_name_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore the character
    } else if (AsciiUpper(c)) {
        std::string lowercase = std::to_string(std::tolower(c));
        CreateToken(DOCTYPEToken, lowercase, "", "", false);
        SwitchTo(DOCTYPEName);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("BeforeDOCTYPEName", "Unexpected null character");
        CreateToken(DOCTYPEToken, std::to_string(REPLACEMENT_CHARACTER), "", "", false);
        SwitchTo(DOCTYPEName);
    } else if (c == '>') {
        TOKENIZER_ERROR("BeforeDOCTYPEName", "Missing DOCTYPE name");
        CreateToken(DOCTYPEToken);
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("BeforeDOCTYPEName", "EOF in doctype");
        CreateToken(DOCTYPEToken);
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        CreateToken(DOCTYPEToken, std::to_string(c), "", "", false);
        SwitchTo(DOCTYPEName);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#doctype-name-state
void Tokenizer::DOCTYPE_name_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(DOCTYPEName);
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (AsciiUpper(c)) {
        append_char_current_doctype_token_name(std::tolower(c));
    } else if (c == cNULL) {
        TOKENIZER_ERROR("DOCTYPEName", "Unexpected null character");
        append_char_current_doctype_token_name(REPLACEMENT_CHARACTER);
    } else if (c == EOF) {
        TOKENIZER_ERROR("DOCTYPEName", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_doctype_token_name(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-name-state
void Tokenizer::after_DOCTYPE_name_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore the character
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("AfterDOCTYPEName", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        if (m_current_position + 6 >= m_content.size()) {
            emit_eof_token();
            return;
        }

        std::string next_characters;
        for (int i = 0; i < 6; ++i) {
            next_characters += std::tolower(peek(i));
        }

        if (next_characters == "public") {
            SwitchTo(AfterDOCTYPEPublicKeyword);
            consume_jump(6);
        } else if (next_characters == "system") {
            SwitchTo(AfterDOCTYPESystemKeyword);
            consume_jump(6);
        } else {
            TOKENIZER_ERROR("AfterDOCTYPEName", "Invalid character sequence after DOCTYPE name");
            set_doctype_force_quirks_flag();
            ReconsumeIn(BogusDOCTYPE);
        }
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-public-keyword-state
void Tokenizer::after_DOCTYPE_public_keyword_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(BeforeDOCTYPEPublicIdentifier);
    } else if (c == '"') {
        TOKENIZER_ERROR("AfterDOCTYPEPublicKeyword",
                        "Missing whitespace after DOCTYPE public keyword");
        set_doctype_public_identifier_not_missing();
        SwitchTo(DOCTYPEPublicIdentifierDoubleQuoted);
    } else if (c == '\'') {
        TOKENIZER_ERROR("AfterDOCTYPEPublicKeyword",
                        "Missing whitespace after DOCTYPE public keyword");
        set_doctype_public_identifier_not_missing();
        SwitchTo(DOCTYPEPublicIdentifierSingleQuoted);
    } else if (c == '>') {
        TOKENIZER_ERROR("AfterDOCTYPEPublicKeyword", "Missing DOCTYPE public identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("AfterDOCTYPEPublicKeyword", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("AfterDOCTYPEPublicKeyword",
                        "Missing quote before DOCTYPE public identifier");
        set_doctype_force_quirks_flag();
        ReconsumeIn(BogusDOCTYPE);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-public-identifier-state
void Tokenizer::before_DOCTYPE_public_identifier_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore the character
    } else if (c == '"') {
        set_doctype_public_identifier_not_missing();
        SwitchTo(DOCTYPEPublicIdentifierDoubleQuoted);
    } else if (c == '\'') {
        set_doctype_public_identifier_not_missing();
        SwitchTo(DOCTYPEPublicIdentifierSingleQuoted);
    } else if (c == '>') {
        TOKENIZER_ERROR("BeforeDOCTYPEPublicIdentifier", "Missing DOCTYPE public identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("BeforeDOCTYPEPublicIdentifier", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("BeforeDOCTYPEPublicIdentifier",
                        "Missing quote before DOCTYPE public identifier");
        set_doctype_force_quirks_flag();
        ReconsumeIn(BogusDOCTYPE);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#doctype-public-identifier-(double-quoted)-state
void Tokenizer::DOCTYPE_public_identifier_double_quoted_state() {
    char c = current_char();

    if (c == '"') {
        SwitchTo(AfterDOCTYPEPublicIdentifier);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("DOCTYPEPublicIdentifierDoubleQuoted", "Unexpected null character");
        append_char_current_doctype_token_public_identifier(REPLACEMENT_CHARACTER);
    } else if (c == '>') {
        TOKENIZER_ERROR("DOCTYPEPublicIdentifierDoubleQuoted", "Abrupt DOCTYPE public identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("DOCTYPEPublicIdentifierDoubleQuoted", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_doctype_token_public_identifier(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#doctype-public-identifier-(double-quoted)-state
void Tokenizer::DOCTYPE_public_identifier_single_quoted_state() {
    char c = current_char();

    if (c == '\'') {
        SwitchTo(AfterDOCTYPEPublicIdentifier);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("DOCTYPEPublicIdentifierSingleQuoted", "Unexpected null character");
        append_char_current_doctype_token_public_identifier(REPLACEMENT_CHARACTER);
    } else if (c == '>') {
        TOKENIZER_ERROR("DOCTYPEPublicIdentifierSingleQuoted", "Abrupt DOCTYPE public identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("DOCTYPEPublicIdentifierSingleQuoted", "Abrupt DOCTYPE public identifier");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_doctype_token_public_identifier(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-public-identifier-state
void Tokenizer::after_DOCTYPE_public_identifier_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(BetweenDOCTYPEPublicAndSystemIdentifiers);
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (c == '"') {
        TOKENIZER_ERROR("AfterDOCTYPEPublicIdentifier",
                        "Missing whitespace between DOCTYPE and system identifiers");
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierDoubleQuoted);
    } else if (c == '\'') {
        TOKENIZER_ERROR("AfterDOCTYPEPublicIdentifier",
                        "Missing whitespace between DOCTYPE and system identifiers");
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierSingleQuoted);
    } else if (c == EOF) {
        TOKENIZER_ERROR("AfterDOCTYPEPublicIdentifier", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("AfterDOCTYPEPublicIdentifier",
                        "Missing quote before DOCTYPE system identifier");
        set_doctype_force_quirks_flag();
        ReconsumeIn(BogusDOCTYPE);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#between-doctype-public-and-system-identifiers-state
void Tokenizer::between_DOCTYPE_public_and_system_identifiers_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore the character
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (c == '"') {
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierDoubleQuoted);
    } else if (c == '\'') {
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierSingleQuoted);
    } else if (c == EOF) {
        TOKENIZER_ERROR("BetweenDOCTYPEPublicAndSystemIdentifiers", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("BetweenDOCTYPEPublicAndSystemIdentifiers",
                        "Missing quote before doctype system identifier");
        set_doctype_force_quirks_flag();
        ReconsumeIn(BogusDOCTYPE);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-system-keyword-state
void Tokenizer::after_DOCTYPE_system_keyword_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        SwitchTo(BeforeDOCTYPESystemIdentifier);
    } else if (c == '"') {
        TOKENIZER_ERROR("AfterDOCTYPESystemKeyword",
                        "Missing whitespace between DOCTYPE and system identifiers");
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierDoubleQuoted);
    } else if (c == '\'') {
        TOKENIZER_ERROR("AfterDOCTYPESystemKeyword",
                        "Missing whitespace between DOCTYPE and system identifiers");
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierSingleQuoted);
    } else if (c == '>') {
        TOKENIZER_ERROR("AfterDOCTYPESystemKeyword", "Missing DOCTYPE system identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("AfterDOCTYPESystemKeyword", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("AfterDOCTYPESystemKeyword",
                        "Missing quote before DOCTYPE system identifier");
        set_doctype_force_quirks_flag();
        ReconsumeIn(BogusDOCTYPE);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#before-doctype-system-identifier-state
void Tokenizer::before_DOCTYPE_system_identifier_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore the character
    } else if (c == '"') {
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierDoubleQuoted);
    } else if (c == '\'') {
        set_doctype_system_identifier_not_missing();
        SwitchTo(DOCTYPESystemIdentifierSingleQuoted);
    } else if (c == '>') {
        TOKENIZER_ERROR("BeforeDOCTYPESystemIdentifierState", "Missing DOCTYPE system identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("BeforeDOCTYPESystemIdentifierState", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("BeforeDOCTYPESystemIdentifierState",
                        "Missing quote before doctype system identifier");
        set_doctype_force_quirks_flag();
        ReconsumeIn(BogusDOCTYPE);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#doctype-system-identifier-(double-quoted)-state
void Tokenizer::DOCTYPE_system_identifier_double_quoted_state() {
    char c = current_char();

    if (c == '"') {
        SwitchTo(AfterDOCTYPESystemIdentifier);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("DOCTYPESystemIdentifierDoubleQuoted", "Unexpected null character");
        append_char_current_doctype_token_system_identifier(REPLACEMENT_CHARACTER);
    } else if (c == '>') {
        TOKENIZER_ERROR("DOCTYPESystemIdentifierDoubleQuoted", "Abrupt DOCTYPE system identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("DOCTYPESystemIdentifierDoubleQuoted", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_doctype_token_system_identifier(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#doctype-system-identifier-(single-quoted)-state
void Tokenizer::DOCTYPE_system_identifier_single_quoted_state() {
    char c = current_char();

    if (c == '\'') {
        SwitchTo(AfterDOCTYPESystemIdentifier);
    } else if (c == cNULL) {
        TOKENIZER_ERROR("DOCTYPESystemIdentifierSingleQuoted", "Unexpected null character");
        append_char_current_doctype_token_system_identifier(REPLACEMENT_CHARACTER);
    } else if (c == '>') {
        TOKENIZER_ERROR("DOCTYPESystemIdentifierSingleQuoted", "Abrupt DOCTYPE system identifier");
        set_doctype_force_quirks_flag();
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("DOCTYPESystemIdentifierSingleQuoted", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        append_char_current_doctype_token_system_identifier(c);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#after-doctype-system-identifier-state
void Tokenizer::after_DOCTYPE_system_identifier_state() {
    char c = current_char();

    if (c == CHARACTER_TABULATION || c == LINE_FEED || c == FORM_FEED || c == SPACE) {
        // Ignore the character
    } else if (c == '>') {
        SwitchTo(Data);
        emit_current_token();
    } else if (c == EOF) {
        TOKENIZER_ERROR("AfterDOCTYPESystemIdentifier", "EOF in DOCTYPE");
        set_doctype_force_quirks_flag();
        emit_current_token();
        emit_eof_token();
    } else {
        TOKENIZER_ERROR("AfterDOCTYPESystemIdentifier",
                        "Unexpected character after DOCTYPE system identifier");
        ReconsumeIn(BogusDOCTYPE);
    }
}

// https://html.spec.whatwg.org/multipage/parsing.html#bogus-doctype-state
void Tokenizer::bogus_DOCTYPE_state() {
    char c = current_char();

    if (c == '>') {
        SwitchTo(Data);
        emit_DOCTYPE_token();
    } else if (c == cNULL) {
        TOKENIZER_ERROR("BogusDOCTYPE", "Unexpected null character");
        // Ignore the character
    } else if (c == EOF) {
        emit_current_token();
        emit_eof_token();
    } else {
        // Ignore the character
    }
}

#pragma endregion

} // namespace liquid::html
