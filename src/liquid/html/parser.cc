#include "parser.h"

void Parser::initial_mode()
{
  Token token = m_tokenizer.current();

  if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
    return;
  else if (token.type == TokenType::Comment)
		return;
  else if (token.type == TokenType::DOCTYPE)
  {
		if (token.value != "html")
			std::cerr << "Parsing error: Token value is not 'html'" << std::endl;
		else
			current_insertion_mode = InsertionMode::BeforeHTML;
  }
	else
	{
		reconsume_token = true;
		current_insertion_mode = InsertionMode::BeforeHTML;
	}
}

void Parser::before_html_mode()
{
	Token token = m_tokenizer.current();

	if (token.type == TokenType::DOCTYPE)
		return;
	else if (token.type == TokenType::Comment)
		return;
	else if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
		return;
	else if (token.type == TokenType::StartTag and token.value == "html")
	{
		HTMLHtmlElement html_element = new HTMLHtmlElement();
		current_insertion_mode = InsertionMode::BeforeHead;
	}
}

Parser::Parser()
{
}

void Parser::parse(Tokenizer& tokenizer)
{
  m_tokenizer = tokenizer;
	reconsume_token = false;
  current_insertion_mode = InsertionMode::Initial;

  while (not m_tokenizer.is_last())
  {
    switch (current_insertion_mode)
    {
      case InsertionMode::Initial:
        initial_mode();
        break;
      case InsertionMode::BeforeHTML:
        before_html_mode();
        break;
      // ...
      default:
        std::cout << "Insertion Mode Unknown" << std::endl;
    }

		if (not reconsume_token)
			m_tokenizer.next();
		else
			reconsume_token = false;
  }
}
