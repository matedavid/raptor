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
	else if (isalpha(c))
	{
		current_token = Token{TokenType::StartTag, ""};

		reconsume();
		current_state = State::TagName;
	}
	else
	{
		std::cerr << "Parsing error: " << c << std::endl;
		m_tokens.push_back(Token{TokenType::Character, "<"});

		reconsume();
		current_state = State::Data;
	}
}

void Tokenizer::consume_end_tag_open_state()
{
	char c = consume();
	if (isalpha(c))
	{
		current_token = Token{TokenType::EndTag, ""};

		reconsume();
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
		exit(1); // TODO: this is not really the appropiate behaviour, bocus_comment_state not implemented
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
		char lower_case = std::tolower(c);
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
	else if (c == '/' or c == '>')
	{
		reconsume();
		current_state = State::AfterAttributeName;
	}
	else if (c == '=')
	{
		std::cerr << "Parsing error: " << c << std::endl;

		current_token.attributes.push_back( Attribute{std::string(1, c), ""} );
		current_state = State::AttributeName;
	}
	else
	{
		current_token.attributes.push_back( Attribute{"", ""} );

		reconsume();
		current_state = State::AttributeName;
	}
}

void Tokenizer::consume_attribute_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ' or c == '/' or c == '>')
	{
		reconsume();
		current_state = State::AfterAttributeName;
	}
	else if (c == '=')
		current_state = State::BeforeAttributeValue;
	else if (c == '>')
		current_state = State::Data;
	else if (c >= 'A' and c <= 'Z')
	{
		char lower_case = std::tolower(c);
		current_token.attributes[current_token.attributes.size()-1].first += lower_case;
	}
	else if (c == '"' or c == '\'' or c == '<')
	{
		std::cerr << "Parsing error: " << c << std::endl;
		current_token.attributes[current_token.attributes.size()-1].first += c;
	}
	else
		current_token.attributes[current_token.attributes.size()-1].first += c;
}

void Tokenizer::consume_after_attribute_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		return;
	else if (c == '/')
		current_state = State::SelfClosingStartTag;
	else if (c == '=')
		current_state = State::BeforeAttributeValue;
	else if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else
	{
		current_token.attributes.push_back( Attribute{"", ""} );

		reconsume();
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
	else
	{
		reconsume();
		current_state = State::UnquotedAttributeValue;
	}
}

void Tokenizer::consume_double_quoted_attribute_value_state()
{
	char c = consume();
	if (c == '"')
		current_state = State::AfterQuotedAttributeValue;
	else
		current_token.attributes[current_token.attributes.size()-1].second += c;
}

void Tokenizer::consume_single_quoted_attribute_value_state()
{
	char c = consume();
	if (c == '\'')
		current_state = State::AfterQuotedAttributeValue;
	else
		current_token.attributes[current_token.attributes.size()-1].second += c;
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
		current_token.attributes[current_token.attributes.size()-1].second += c;
	}
	else
		current_token.attributes[current_token.attributes.size()-1].second += c;
}

void Tokenizer::consume_after_quoted_attribute_value_state()
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
	else
	{
		std::cerr << "Parsing error: " << c << std::endl;

		reconsume();
		current_state = State::BeforeAttributeName;
	}
}

void Tokenizer::consume_self_closing_start_tag_state()
{
	char c = consume();
	if (c == '>')
	{
		current_token.self_closing = true;
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else
	{
		std::cerr << "Parsing error (self_closing_start_tag): " << c << std::endl;

		reconsume();
		current_state = State::BeforeAttributeValue;
	}
}

void Tokenizer::consume_markup_declaration_open_state()
{
	if (peek_consume_forward(2) == "--")
	{
		m_position += 2+1;
		current_token = Token{TokenType::Comment, ""};
		current_state = State::CommentStart;
	}
	else if (peek_consume_forward(7) == "DOCTYPE")
	{
		m_position += 7+1;
		current_state = State::DOCTYPE;
	}
	else
	{
		std::cerr << "Parsing error (markup_declaration_open): " << m_content[m_position] << std::endl;
		exit(1);
	}
}

void Tokenizer::consume_comment_start_state()
{
	char c = consume();
	if (c == '-')
		current_state = State::CommentStartDash;
	else if (c == '>')
	{
		std::cerr << "Parsing error (start_state): " << c << std::endl;

		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else
	{
		reconsume();
		current_state = State::Comment;
	}
}

void Tokenizer::consume_comment_start_dash_state()
{
	char c = consume();
	if (c == '-')
		current_state = State::CommentEnd;
	else if (c == '>')
	{
		std::cerr << "Parsing error (comment_start_dash): " << c << std::endl;

		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else
	{
		current_token.value += '-';

		reconsume();
		current_state = State::Comment;
	}
}

void Tokenizer::consume_comment_state()
{
	char c = consume();
	if (c == '<')
	{
		current_token.value += c;
		// Note(david): Should change to CommentLessThanSignState, but this state does not exist for the moment
	}
	else if (c == '-')
		current_state = State::CommentEndDash;
	else
		current_token.value += c;
}

void Tokenizer::consume_comment_end_dash_state()
{
	char c = consume();
	if (c == '-')
		current_state = State::CommentEnd;
	else
	{
		current_token.value += '-';

		reconsume();
		current_state = State::Comment;
	}
}

void Tokenizer::consume_comment_end_state()
{
	char c = consume();
	if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else if (c == '!')
	{
		// Note(david): CommentEndBangState is not implemented
	}
	else if (c == '-')
		current_token.value += '-';
	else
	{
		current_token.value += "--";

		reconsume();
		current_state = State::Comment;
	}
}

void Tokenizer::consume_DOCTYPE_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		current_state = State::BeforeDOCTYPEName;
	else if (c == '>')
	{
		reconsume();
		current_state = State::BeforeDOCTYPEName;
	}
	else
	{
		reconsume();
		current_state = BeforeDOCTYPEName;
	}
}

void Tokenizer::consume_before_DOCTYPE_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		return;
	else if (c >= 'A' and c <= 'Z')
	{
		char lower_case = std::tolower(c);
		current_token = Token{TokenType::DOCTYPE, std::string(1, lower_case)};
		current_state = State::DOCTYPEName;
	}
	else if (c == '>')
	{
		std::cout << "Parsing error (before_DOCTYPE_name): " << c << std::endl;

		// TODO(david): Set force-quirks of token to on ???
		current_token = Token{TokenType::DOCTYPE};
		m_tokens.push_back(current_token);

		current_state = State::Data;
	}
	else
	{
		current_token = Token{TokenType::DOCTYPE, std::string(1, c)};
		current_state = State::DOCTYPEName;
	}
}

void Tokenizer::consume_DOCTYPE_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		current_state = State::DOCTYPEName;
	else if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else if (c >= 'A' and c <= 'Z')
	{
		char lower_case = std::tolower(c);
		current_token.value += lower_case;
	}
	else
		current_token.value += c;
}

void Tokenizer::consume_after_DOCTYPE_name_state()
{
	char c = consume();
	if (c == '\t' or c == '\n' or c == '\f' or c == ' ')
		return;
	else if (c == '>')
	{
		current_state = State::Data;
		m_tokens.push_back(current_token);
	}
	else
	{
		// TODO(david): Implement this part
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

void Tokenizer::reconsume()
{
	if (m_position-1 >= 0)
		--m_position;
}

std::string Tokenizer::peek_consume_forward(int n_chars)
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

std::string Tokenizer::peek_consume_backwards(int n_chars)
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
			case State::MarkupDeclarationOpenState:
				consume_markup_declaration_open_state();
				break;
			case State::CommentStart:
				consume_comment_state();
				break;
			case State::CommentStartDash:
				consume_comment_start_dash_state();
				break;
			case State::Comment:
				consume_comment_state();
				break;
			case State::CommentEndDash:
				consume_comment_end_dash_state();
				break;
			case State::CommentEnd:
				consume_comment_end_state();
				break;
    	case State::DOCTYPE:
				consume_DOCTYPE_state();
				break;
			case State::BeforeDOCTYPEName:
				consume_before_DOCTYPE_name_state();
				break;
			case State::DOCTYPEName:
				consume_DOCTYPE_name_state();
				break;
    	case State::AfterDOCTYPEName:
				consume_after_DOCTYPE_name_state();
				break;
			default:
				std::cout << "State not recognized" << std::endl;
		}
  }

	for (int i = 0; i < m_tokens.size(); ++i)
	{
		Token token = m_tokens[i];
		std::cout << token_type_as_string(token.type) << " " << token.value;
		if (token.attributes.size() != 0)
		{
			std::cout << " [";
			for (int i = 0; i < token.attributes.size(); ++i)
				std::cout << "(" << token.attributes[i].first << ", " << token.attributes[i].second << ")";
			std::cout << "]";
		}
		std::cout << std::endl;
	}

}
