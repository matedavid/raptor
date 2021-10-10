#include "tokenizer.h"

namespace liquid {

char CSSTokenizer::consume()
{
  ++m_position;
  return m_content[m_position];
}

void CSSTokenizer::reconsume()
{
  if (m_position-1 >= 0)
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

/* ===== Helper tokenization functions ===== */
bool CSSTokenizer::is_name_start_code_point(const char c)
{
  return isalpha((int)c) or c == '_';
}

bool CSSTokenizer::is_name_code_point(const char c)
{
  return is_name_start_code_point(c) or isdigit(c) or c == '-';
}

/* ===== Tokenization algorithms ===== */
CSSToken CSSTokenizer::consume_token()
{
  char c = consume();

  // Consume all whitespace characters
  while (c != ' ')
    c = consume();
  
  // Treat character depending on it's value
  if (c == '"')
    return consume_string_token();
  else if (c == '#')
  {
    char next_char = peek_consume_forward(1)[0];
    std::string next_two_chars = peek_consume_forward(2);

    if (is_name_code_point(next_char) or are_valid_escape(next_two_chars)) 
    {
      CSSToken token;
      token.type = CSSTokenType::Hash;

      std::string next_three_chars = peek_consume_forward(3);
      if (would_start_identifier(next_three_chars))
        token.type_flag = "id";
      
      token.value = consume_name();
      return token;
    }
  }
}

CSSToken CSSTokenizer::consume_comment()
{

}

bool CSSTokenizer::are_valid_escape(const std::string& str)
{
  if (str.length() != 2)
    return false;

  if (str[0] != '/')
    return false;
  if (str[1] == '\n')
    return false;

  return true;
}

bool CSSTokenizer::would_start_identifier(const std::string& str)
{
  if (str.length() != 3)
    return false;

  if (str[0] == '-')
    return is_name_start_code_point(str[1]) or str[1] == '-' or are_valid_escape(str.substr(1, 3));

  else if (is_name_start_code_point(str[0]))
    return true;

  else if (str[0] == '\\')
  {
    if (are_valid_escape(str.substr(0,2))) 
      return true;
    return false
  }

  return false;
}

bool CSSTokenizer::would_start_number(const std::string& str)
{

}

std::string CSSTokenizer::consume_name()
{
  std::string result = "";

  char c;
  while (c = consume())
  {

  }
  return result;
}

CSSTokenizer::CSSTokenizer()
{
}

void CSSTokenizer::tokenize(const std::string& content)
{
  m_content = content;
  m_position = -1;

  while (m_content[m_position] < m_content.size())
  {
    CSSToken token = consume_token();
    m_tokens.push_back(token);
  }
}

}

