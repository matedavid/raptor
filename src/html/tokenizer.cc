#include "tokenizer.h"

Tokenizer::Tokenizer()
{
}

Tokenizer::Tokenizer(const std::string& content)
					: m_content(content)
{
}

void Tokenizer::tokenize_text()
{
	std::string text_value = "";
	while (isalpha(m_content[m_position]) or isdigit(m_content[m_position]) or (m_tokens[m_tokens.size()-1].type == GREATER_THAN_TOKEN and m_content[m_position] == ' '))
	{
		text_value += m_content[m_position];
		++m_position;
	}

	Token token;
	token.type = TOKEN_TYPE_from_string(text_value);
	token.value = text_value;

	m_tokens.push_back(token);
}

void Tokenizer::tokenize_string()
{
	char expected_c = m_content[m_position];
	++m_position; // Jump stargint " or ' character

	std::string string_value;
	while (m_content[m_position] != expected_c)
	{
		string_value += m_content[m_position];
		++m_position;
	}
	++m_position; // Jump second " or ' character

	Token token = Token{TEXT_TOKEN, string_value};
	m_tokens.push_back(token);
}

void Tokenizer::tokenize_special_character()
{
	char c = m_content[m_position];
	Token token;
	token.type = TOKEN_TYPE_from_special_character(c);
	token.value = "";

	++m_position; // Jump special character
	m_tokens.push_back(token);
}

void Tokenizer::tokenize()
{
	if (m_content.empty())
		std::cout << "HTML: Can't tokenize file, no HTML string provided" << std::endl;
	else
		tokenize(m_content);
}

void Tokenizer::tokenize(const std::string& content)
{
	m_content = content;

	m_position = 0;
	while (m_position < m_content.length())
	{
		char c = m_content[m_position];
		if (isalpha(c) or isdigit(c))
			tokenize_text();
		else if (c == '"' or c == '\'')
			tokenize_string();
		else if (c != ' ' and c != '\t' and c != '\n')
			tokenize_special_character();
		else
			++m_position;
	}

	// Add EOF token as the last token
	m_tokens.push_back(Token{EOF_TOKEN, ""});
	// Reset m_position for further use of the class
	m_position = 0;
}

const Token Tokenizer::current() const
{
	return m_tokens[m_position];
}

void Tokenizer::next()
{
	if (m_position+1 < m_tokens.size())
		++m_position;
}

void Tokenizer::prev()
{
	if (m_position-1 >= 0)
		--m_position;
}

const Token Tokenizer::peek_next() const
{
	if (m_position+1 < m_tokens.size())
		return m_tokens[m_position+1];
	return m_tokens[m_position];
}

const Token	 Tokenizer::peek_prev() const
{
	if (m_position-1 >= 0)
		return m_tokens[m_position-1];
	return m_tokens[m_position];
}
