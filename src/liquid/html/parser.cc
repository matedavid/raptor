#include "parser.h"

void Parser::text_mode()
{
  Token token = m_tokenizer.current();

  if (token.type == TokenType::Character)
  {
    HTMLElement* last_element = document.html->get_last_element();
    if (last_element->type() == HTMLElementType::TextType)
    {
      Text* last_text_element = dynamic_cast<Text*>(last_element);
      last_text_element->append_string(token.value);
    } 
    else
    {
      Text* text_element = new Text(token.value);
      document.html->insert_child_last(text_element);
    }
  }
  else if (token.type == TokenType::EOFToken)
  {
    std::cerr << "Parsing error: EOF token in text_mode" << std::endl;
    open_elements.pop();

    reconsume_token = true;
    current_insertion_mode = original_insertion_mode;
  }
  else if (token.type = TokenType::EndTag)
  {
    open_elements.pop();
    current_insertion_mode = original_insertion_mode;
  }
}

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
		document.html->insert_child(head_element);

    open_elements.push(head_element);

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
		document.html->insert_child(head_element);

    open_elements.push(head_element);

		document.head = head_element;
		reconsume_token = true;
		current_insertion_mode = InsertionMode::InHead;
	}
}

void Parser::in_head_mode()
{
  Token token = m_tokenizer.current();

	if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
  {
    HTMLElement* last_element = document.head->get_last_element();
		if (last_element->type() == HTMLElementType::TextType)
		{
			Text* text_last_element = dynamic_cast<Text*>(last_element);
			text_last_element->append_string(token.value);
		}
		else
		{
			Text* text_element = new Text(token.value);
			document.head->insert_child_last(text_element);
		}
  }
  else if (token.type == TokenType::Comment)
    return;
  else if (token.type == TokenType::DOCTYPE)
  {
    std::cerr << "Parsing error: DOCTYPE token in in_head insertion mode" << std::endl;
    return;
  }
  else if (token.type == TokenType::StartTag and token.value == "html")
  {
    reconsume_token = true;
    current_insertion_mode = InsertionMode::InBody;
  }
  else if ( token.type == TokenType::StartTag and (token.value == "base" or token.value == "basefont" or token.value == "bgsound" or token.value == "link") )
  {
    // TODO(david): Implement
  }
  else if (token.type == TokenType::StartTag and token.value == "meta")
  {
    // TODO(david): Implement
  }
  else if (token.type == TokenType::StartTag and token.value == "title")
  {
    HTMLTitleElement* title_element = new HTMLTitleElement(token, nullptr);
    document.head->insert_child_last(title_element);

    original_insertion_mode = current_insertion_mode;
    current_insertion_mode = InsertionMode::TextMode;
  }
  // TODO(david): Missing multple parts, not implementing at the moment
  else if (token.type == TokenType::EndTag and token.value == "head")
  {
    open_elements.pop();
    current_insertion_mode = InsertionMode::AfterHead;
  }
  else if (token.type == TokenType::StartTag and token.value == "head")
  {
    std::cerr << "Parsing error: start head tag in in_head insertion mode" << std::endl;
    return;
  }
  else if ( token.type == TokenType::EndTag and (token.value == "body" or token.value == "html" or token.value == "br") )
  {
    open_elements.pop();

    reconsume_token = true;
    current_insertion_mode = InsertionMode::AfterHead;
  } 
  else 
  {
    open_elements.pop();

    reconsume_token = true;
    current_insertion_mode = InsertionMode::AfterHead;
  }
}

void Parser::after_head_mode()
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
      case InsertionMode::TextMode:
        text_mode();
        break;
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
      case InsertionMode::AfterHead:
        after_head_mode();
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

	std::cout << "Printing AST result from parsing:" << std::endl;
	// document.html->print();
  print_html_element(document.html, 0);
}
