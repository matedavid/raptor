#include "tokenizer.h"

void Tokenizer::consume_data_state()
{
	char c = consume();
	if (c == '<')
		current_state = State::TagOpen;
	else
		m_tokens.push_back(Token{TokenType::Character, std::string(1, c)});
}

void Tokenizer::consume_tag_open_state()
{
	char c = consume();
	if (c == '!')
		current_state = State::MarkupDeclarationOpenState;
	else if (c == '/')
		current_state = State::EndTagOpen;
	else if (c >= 'A' and c <= 'Z') // Is capital letter
	{
		char lower_case = (c - 'A') + 'a';
		current_token = Token{TokenType::StartTag, std::string(1, lower_case)};
		current_state = State::TagName;
	}
	else if (c >= 'a' and c <= 'z') // Is lowercase letter
	{
		current_token = Token{TokenType::StartTag, std::string(1, c)};
		current_state = State::TagName;
	}
	else
	{
		std::cerr << "Parsing error: " << c << std::endl;
		--m_position;
		consume_data_state();
	}
}

void Tokenizer::consume_end_tag_open_state()
{
	char c = consume();
	if (c >= 'A' and c <= 'Z') // Is capital letter
	{
		char lower_case = (c - 'A') + 'a';
		current_token = Token{TokenType::EndTag, std::string(1, lower_case)};
		current_state = State::TagName;
	}
	else if (c >= 'a' and c <= 'z')
	{
		current_token = Token{TokenType::EndTag, std::string(1, c)};
		current_state = State::TagName;
	}
	else if (c == '>')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_state = State::Data;
	}
	else
	{
		std::cerr << "Parsing error: " << c << std::endl;
		exit(1);
	}
}

void Tokenizer::consume_tag_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		current_state = State::BeforeAttributeName;
	else if (c == '/')
		current_state = State::SelfClosingStartTag;
	else if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else if (c >= 'A' and c <= 'Z')
	{
		char lower_case = (c - 'A') + 'a';
		current_token.value += lower_case;
	}
	else
		current_token.value += c;
}

void Tokenizer::consume_before_attribute_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		return;
	else if (c == '/')
		current_state = State::SelfClosingStartTag;
	else if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else if (c >= 'A' and c <= 'Z')
	{
		char lower_case = (c - 'A') + 'a';
		current_attribute = Attribute{std::string(1, lower_case), ""};
	}
	else if (c == '"' or c == '\'' or c == '<' or c == '=')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_attribute = Attribute{std::string(1, c), ""};
		current_state = State::AttributeName;
	}
	else
	{
		current_attribute = Attribute{std::string(1, c), ""};
		current_state = State::AttributeName;
	}
}

void Tokenizer::consume_attribute_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		current_state = State::AfterAttributeName;
	else if (c == '/')
		current_state = State::SelfClosingStartTag;
	else if (c == '=')
		current_state = State::BeforeAttributeValue;
	else if (c == '>')
		current_state = State::Data;
	else if (c >= 'A' and c <= 'Z')
	{
		char lower_case = (c - 'A') + 'a';
		current_attribute.first += lower_case;
	}
	else if (c == '"' or c == '\'' or c == '<')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_attribute.first += c;
	}
	else
		current_attribute.first += c;
}

void Tokenizer::consume_after_attribute_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		return;
	else if (c == '/')
		current_state = State::SelfClosingStartTag;
	else if (c == '=')
		current_state = State::BeforeAttributeName;
	else if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else if (c >= 'A' and c <= 'Z')
	{
		char lower_case = (c - 'A') + 'a';
		current_attribute = Attribute{std::string(1, lower_case), ""};
		current_state = State::AttributeName;
	}
	else if (c == '"' or c == '\'' or c == '<')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_attribute = Attribute{std::string(1, c), ""};
		current_state = State::AttributeName;
	}
	else
	{
		current_attribute = Attribute{std::string(1, c), ""};
		current_state = State::AttributeName;
	}
}

void Tokenizer::consume_before_attribute_value_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		return;
	else if (c == '"')
		current_state = State::DoubleQuotedAttributeValue;
	else if (c == '\'')
		current_state = State::SingleQuotedAttributeValue;
	else if (c == '>')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else if (c == '<' or c == '=' or c == '`')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_attribute.second += c;
		current_state = State::UnquotedAttributeValue;
	}
	else
	{
		current_attribute.second += c;
		current_state = State::UnquotedAttributeValue;
	}
}

void Tokenizer::consume_double_quoted_attribute_value_state()
{
	char c = consume();
	if (c == '"')
		current_state = State::AfterQuotedAttributeValue;
	else
		current_attribute.second += c;
}

void Tokenizer::consume_single_quoted_attribute_value_state()
{
	char c = consume();
	if (c == '\'')
		current_state = State::AfterQuotedAttributeValue;
	else
		current_attribute.second += c;
}

void Tokenizer::consume_unquoted_attribute_value_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		current_state = State::BeforeAttributeName;
	else if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else if (c == '"' or c == '\'' or c == '<' or c == '=' or c == '`')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_attribute.second += c;
	}
	else
		current_attribute.second += c;
}

void Tokenizer::consume_after_quoted_attribute_value_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		current_state = State::BeforeAttributeName;
	else if (c == '/')
		current_state = State::SelfClosingStartTag;
	else if (c == '>')
		current_state = State::Data;
	else
	{
		std::cerr << "Parsing error: " << c << std::endl;

		--m_position;
		consume_before_attribute_name_state();
	}
}

void Tokenizer::consume_self_closing_start_tag_state()
{
	char c = consume();
	if (c == '>')
	{
		current_token.self_closing = true;
		current_state = State::Data;
	}
	else
	{
		std::cerr << "Parsing error: " << c << std::endl;
		--m_position;
		consume_before_attribute_name_state();
	}
}




void Tokenizer::consume_markup_declaration_open_state()
{
	if (peek_consume_forward(2) == "--")
	{
		current_token = Token{TokenType::Comment, ""};
		current_state = State::CommentStart;
	}
	else if (peek_consume_forward(7) == "DOCTYPE")
	{
		m_position += 7;
		current_state = State::DOCTYPE;
	}
	else
	{
		std::cerr << "Parsing error: " << m_content[m_position] << std::endl;
		exit(1);
	}
}

Tokenizer::Tokenizer()
{
}

char Tokenizer::consume()
{
  ++m_position;
  return m_content[m_position];
}

std::string Tokenizer::peek_consume_forward(int n_chars)
{
	std::string peek_string = "";
	for (int i = 0; i < n_chars; ++i)
	{
		if (m_position+i >= m_content.length())
			break;
		peek_string += m_content[m_position+i];
	}

	return peek_string;
}

std::string Tokenizer::peek_consume_backwards(int n_chars)
{
	std::string peek_string = "";
	for (int i = 0; i < n_chars; ++i)
	{
		if (m_position-i < 0)
			break;
		peek_string += m_content[m_position-i];
	}

	return peek_string;
}

void Tokenizer::tokenize(const std::string& content)
{
  m_content = content;
	m_position = -1;
	current_state = State::Data;

	int length = m_content.length();
	while (m_position < length)
  {
		switch (current_state)
		{
			case State::Data:
				consume_data_state();
				break;
			case State::TagOpen:
				consume_tag_open_state();
				break;
			case State::EndTagOpen:
				consume_end_tag_open_state();
				break;
			case State::TagName:
				consume_tag_name_state();
				break;
			case State::BeforeAttributeName:
				consume_before_attribute_name_state();
				break;
			case State::AttributeName:
				consume_attribute_name_state();
				break;
			case State::AfterAttributeName:
				consume_after_attribute_name_state();
				break;
			case State::BeforeAttributeValue:
				consume_before_attribute_value_state();
				break;
			case State::DoubleQuotedAttributeValue:
				consume_double_quoted_attribute_value_state();
				break;
			case State::SingleQuotedAttributeValue:
				consume_single_quoted_attribute_value_state();
				break;
			case State::UnquotedAttributeValue:
				consume_unquoted_attribute_value_state();
				break;
			case State::AfterQuotedAttributeValue:
				consume_after_quoted_attribute_value_state();
				break;
			case State::SelfClosingStartTag:
				consume_self_closing_start_tag_state();
				break;
			default:
				std::cout << "State not recognized" << std::endl;
		}
  }

	for (int i = 0; i < m_tokens.size(); ++i)
	{
		Token token = m_tokens[i];
		std::cout << token_type_as_string(token.type) << " " << token.value << " " << token.self_closing << std::endl;
	}

}
