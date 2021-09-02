#include "tokenizer.h"

Tokenizer::Tokenizer()
{
}

void Tokenizer::tokenize_tag()
{
	std::string tag_name = "";

	++m_position; // Jump '<' character
	while (m_content[m_position] != '>')
	{
		tag_name += m_content[m_position];
		++m_position;
	}
	++m_position; // Jump '>' character

	bool is_closing_tag = false;
	if (tag_name[0] == '/')
	{
		is_closing_tag = true;
		tag_name.erase(0,1);
	}

	enum Token_Type type;
	if (tag_name == "html")
		type = HTML_TAG;
	else if (tag_name == "head")
		type = HEAD_TAG;
	else if (tag_name == "body")
		type = BODY_TAG;
	else if (tag_name == "title")
		type = TITLE_TAG;
	else if (tag_name == "p")
		type = P_TAG;
	else if (tag_name == "h1")
		type = H1_TAG;
	else if (tag_name == "h2")
		type = H2_TAG;
	else if (tag_name == "h3")
		type = H3_TAG;
	else if (tag_name == "h4")
		type = H4_TAG;
	else if (tag_name == "h5")
		type = H5_TAG;
	else if (tag_name == "h6")
		type = H6_TAG;
	else
		type = UNKNOWN_TAG;

	Token token = Token(type, tag_name, is_closing_tag);
	m_tokens.push_back(token);
}

void Tokenizer::tokenize_text()
{
	std::string text = "";
	while (m_content[m_position] != '<')
	{
		text += m_content[m_position];
		++m_position;
	}

	Token token = Token(TEXT_TOKEN, text, false);
	m_tokens.push_back(token);
}

void Tokenizer::tokenize(const std::string& content)
{
	m_content = content;
	m_position = 0;

	while (m_position < m_content.length())
	{
		if (m_content[m_position] == '<')
		{
			tokenize_tag();
		}
		else if (isalpha(m_content[m_position]) or isdigit(m_content[m_position]))
		{
			tokenize_text();
		}
		else
		{
			++m_position;
		}
	}
}
