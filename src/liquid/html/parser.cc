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
		HTMLHtmlElement* html_element = new HTMLHtmlElement(token, nullptr);
		document.html = html_element;

		current_insertion_mode = InsertionMode::BeforeHead;
		open_elements.push(html_element);
	}
	else if (token.type == TokenType::EndTag)
		std::cerr << "Parsing error: Before html end tag" << std::endl;
	else if ( token.type == TokenType::EndTag and (token.value == "head" or token.value == "body" or token.value == "html" or token.value == "br") )
	{
		HTMLHtmlElement* html_element = new HTMLHtmlElement(nullptr);
		document.html = html_element;

		reconsume_token = true;
		current_insertion_mode = InsertionMode::BeforeHead;
	}
	else
	{
		HTMLHtmlElement* html_element = new HTMLHtmlElement(nullptr);
		document.html = html_element;

		reconsume_token = true;
		current_insertion_mode = InsertionMode::BeforeHead;
	}
}

void Parser::before_head_mode()
{
	Token token = m_tokenizer.current();

	if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
		return;
	else if (token.type == TokenType::Comment)
		return;
	else if (token.type == TokenType::DOCTYPE)
	{
		std::cerr << "Parsing error: DOCTYPE token y before head mode" << std::endl;
		return;
	}
	else if (token.type == TokenType::StartTag and token.value == "html")
	{
		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
	else if (token.type == TokenType::StartTag and token.value == "head")
	{
		HTMLHeadElement* head_element = new HTMLHeadElement(token, document.html);
		document.head = head_element;
		current_insertion_mode = InsertionMode::InHead;
	}
	else if (token.type == TokenType::EndTag)
	{
		std::cerr << "Parsing error: end tag in before_head mode" << std::endl;
		return;
	}
	else
	{
		HTMLHeadElement* head_element = new HTMLHeadElement(document.html);
		document.head = head_element;

		reconsume_token = true;
		current_insertion_mode = InsertionMode::InHead;
	}
}

void Parser::in_head_mode()
{
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
			case InsertionMode::BeforeHead:
				before_head_mode();
				break;
			case InsertionMode::InHead:
				in_head_mode();
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
