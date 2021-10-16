#include "tokenizer.h"

namespace liquid {

char CSSTokenizer::consume()
{
  ++m_position;
  return m_content[m_position];
}

void CSSTokenizer::reconsume()
{
  if (m_position-1 >= -1)
    --m_position;
}

std::string CSSTokenizer::peek_consume_forward(int n_chars)
{
	std::string peek_string = "";
	for (int i = 1; i <= n_chars; ++i)
	{
		if (m_position+i >= m_content.length())
			break;
		peek_string += m_content[m_position+i];
	}

	return peek_string;
}

std::string CSSTokenizer::peek_consume_backwards(int n_chars)
{
	std::string peek_string = "";
	for (int i = 1; i <= n_chars; ++i)
	{
		if (m_position-i < 0)
			break;
		peek_string += m_content[m_position-i];
	}

	return peek_string;
}


char CSSTokenizer::next_non_white_character()
{
	int tmp_position = m_position;
	while (m_content[tmp_position] == ' ')
		++tmp_position;

	return m_content[tmp_position];
}

void CSSTokenizer::consume_before_selector()
{
	char c = consume();

	if (isalpha(c) or isdigit(c))
	{
		reconsume();
		m_current_token = CSSToken{CSSTokenType::Selector, ""};
		m_current_state = State::InSelector;
	}
	else if (c == '*' or c == '.' or c == '#')
	{
		std::string str_c = std::string(1, c);
		m_tokens.push_back(CSSToken{CSSTokenType::SelectorOption, str_c});
	}
	else if (c == '{')
	{
		std::cout << "Parsing error: '{' character in before_selector mode" << std::endl;

		m_tokens.push_back(CSSToken{CSSTokenType::Selector, "*"});
		m_current_state = State::InBlock;
	}
	else if (c == ' ' or c == '\t' or c == '\f' or c == '\n')
	{ 
		//Ignore the token 
	}
	else if (c == '}')
	{
		// Ignore the token
	}
	else
	{
		// Unknown character
		std::cout << "Parsing error: Unknown character (" << c << ") in before_selector mode" << std::endl;
		exit(1);
	}
}

void CSSTokenizer::consume_in_selector()
{
	char c = consume();

	if (isalpha(c) or isdigit(c) or c == '.')
	{
		m_current_token.value += c;
	}
	else if (c == ',')
	{
		// Add currently tokenizing token 
		m_tokens.push_back(m_current_token);
		// Add comma character as a Selector Option, to identify that block applies to group of selectors
		m_tokens.push_back(CSSToken{CSSTokenType::SelectorOption, ","});

		m_current_state = State::BeforeSelector;
	}
	else if (c == ' ' and next_non_white_character() != '{')
	{
		// Add currently tokenizing token 
		m_tokens.push_back(m_current_token);
		// Add space character as a Selector Option, to identify that block applies to group of selectors
		m_tokens.push_back(CSSToken{CSSTokenType::SelectorOption, " "});

		m_current_state = State::BeforeSelector;
	}
	else if (c == '{')
	{
		m_tokens.push_back(m_current_token);
		
		// Insert BlockStart token
		m_tokens.push_back(CSSToken{CSSTokenType::BlockStart});

		m_current_state = State::InBlock;
	}
	else
	{
		// Ignore character
	}
}

void CSSTokenizer::consume_in_block()
{
	char c = consume();

	if (isalpha(c) or isdigit(c))
	{
		m_tokens.push_back(CSSToken{CSSTokenType::DeclarationStart});
		m_current_token = CSSToken{CSSTokenType::Property, ""};

		reconsume();
		m_current_state = State::InProperty;
	}
	else if (c == ':')
	{
		std::cout << "Parsing error: ':' character before property" << std::endl;
		// Exiting on parse error because of concerns on how to deal with it
		exit(1);
	}
	else if (c == '}')
	{
		// Exit out of block
		m_tokens.push_back(CSSToken{CSSTokenType::BlockEnd});
		m_current_state = State::BeforeSelector;
	}
	else if (c == ';')
	{
		// Ignore token
	}
	else if (c == ' ' or c == '\t' or c == '\f' or c == '\n')
	{
		// Ignore token
	}
}

void CSSTokenizer::consume_in_property()
{
	char c = consume();

	if (isalpha(c) or isdigit(c) or c == '-')
	{
		m_current_token.value += c;
	}
	else if (c == ':')
	{
		m_tokens.push_back(m_current_token);

		m_current_token = CSSToken{CSSTokenType::Value, ""};
		m_current_state = State::InValue;
	}
	else if (c == ';')
	{
		std::cout << "Parsing error: no value for property " << m_current_token.value << std::endl;
		m_current_state = State::InBlock;
	}
	else if (c == ' ' or c == '\t' or c == '\f' or c == '\n')
	{
		// Ignore token
	}
}

void CSSTokenizer::consume_in_value()
{
	char c = consume();
	char next_nw = next_non_white_character();

	if (isalpha(c) or isdigit(c) or c == '-')
	{
		m_current_token.value += c;
	}
	else if (c == ';')
	{
		m_tokens.push_back(m_current_token);
		m_tokens.push_back(CSSToken{CSSTokenType::DeclarationEnd});

		m_current_state = State::InBlock;
	}
	else if (c == ' ' and (isalpha(next_nw) or isdigit(next_nw) or next_nw == '-') and peek_consume_backwards(1) != ":")
	{
		m_tokens.push_back(m_current_token);
		m_current_token = CSSToken{CSSTokenType::Value, ""};
	}
	else if (c == ' ' or c == '\t' or c == '\f' or c == '\n')
	{
		// Ignore token
	}
	else
	{
		std::cout << "Parsing error: Unknwon token (" << c << ") in in_value state" << std::endl;
	}
}

CSSTokenizer::CSSTokenizer()
{
}

void CSSTokenizer::tokenize(const std::string& content)
{
  m_content = content;
  m_position = -1;

	m_current_state = State::BeforeSelector;

	int length = m_content.length();
  while (m_position < length-1) // -1 to ignore EOF token
  {
		switch (m_current_state)
		{
			case State::BeforeSelector:
				consume_before_selector();
				break;
			case State::InSelector:
				consume_in_selector();
				break;
			case State::InBlock:
				consume_in_block();
				break;
			case State::InProperty:
				consume_in_property();
				break;
			case State::InValue:
				consume_in_value();
				break;

			default:
				std::cout << "Unknown Token" << std::endl;
				++m_position;
		}
  }

	// Reset position for further use of the Tokenizer class
	m_position = 0;

	/*
	for (auto token : m_tokens) 
		std::cout << css_token_type_as_string(token.type) << ": '" << token.value << "'" << std::endl;
	*/
}

CSSToken CSSTokenizer::current() const
{
	return m_tokens[m_position];
}

CSSToken CSSTokenizer::next()
{
	if (m_position+1 < m_tokens.size())
		++m_position;

	return m_tokens[m_position];
}

CSSToken CSSTokenizer::prev()
{
	if (m_position-1 >= 0)
		--m_position;

	return m_tokens[m_position];
}

bool CSSTokenizer::is_last() const 
{
	return m_position == m_tokens.size()-1;
}

}
