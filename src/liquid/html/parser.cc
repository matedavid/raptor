#include "parser.h"

void Parser::insert_character(const Token& token, HTMLElement* element)
{
	HTMLElement* last_element = element->get_last_element();
	if (last_element->type() == HTMLElementType::TextType)
	{
		Text* text_last_element = dynamic_cast<Text*>(last_element);
		text_last_element->append_string(token.value);
	}
	else
	{
		Text* text_element = new Text(token.value);
		last_element->insert_child_last(text_element);
	}
}

void Parser::text_mode()
{
  Token token = m_tokenizer.current();

  if (token.type == TokenType::Character)
  {
		HTMLElement* insert_element = open_elements.top();
		insert_character(token, insert_element);
  }
  else if (token.type == TokenType::EOFToken)
  {
    std::cerr << "Parsing error: EOF token in text_mode" << std::endl;
    open_elements.pop();

    reconsume_token = true;
    current_insertion_mode = original_insertion_mode;
  }
	else if (token.type == TokenType::EndTag)
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
		open_elements.push(html_element);

		current_insertion_mode = InsertionMode::BeforeHead;
	}
	else if (token.type == TokenType::EndTag)
		std::cerr << "Parsing error: Before html end tag" << std::endl;
	else if ( token.type == TokenType::EndTag and (token.value == "head" or token.value == "body" or token.value == "html" or token.value == "br") )
	{
		HTMLHtmlElement* html_element = new HTMLHtmlElement(nullptr);
		document.html = html_element;
		open_elements.push(html_element);

		reconsume_token = true;
		current_insertion_mode = InsertionMode::BeforeHead;
	}
	else
	{
		HTMLHtmlElement* html_element = new HTMLHtmlElement(nullptr);
		document.html = html_element;
		open_elements.push(html_element);

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
		HTMLElement* insert_element = open_elements.top();
		insert_character(token, insert_element);
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
		//document.head->insert_child(title_element);
		HTMLElement* insert_element = open_elements.top();
		insert_element->insert_child(title_element);

		open_elements.push(title_element);

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
	Token token = m_tokenizer.current();
	if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
	{
		HTMLElement* insert_element = open_elements.top();
		insert_character(token, insert_element);
	}
	else if (token.type == TokenType::Comment)
		return;
	else if (token.type == TokenType::DOCTYPE)
	{
		std::cerr << "Parsing error: DOCTYPE token in after_head mode" << std::endl;
		return;
	}
	else if (token.type == TokenType::StartTag and token.value == "html")
	{
		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
	else if (token.type == TokenType::StartTag and token.value == "body")
	{
		HTMLBodyElement* body_element = new HTMLBodyElement(token, document.html);
		document.html->insert_child(body_element);
		document.body = body_element;

		open_elements.push(body_element);

		current_insertion_mode = InsertionMode::InBody;
	}
	// TODO: Frameset
	// TODO: base, basefont, bgsound, link, meta, noframes...
	else if (token.type == TokenType::EndTag and token.value == "template")
	{
		reconsume_token = true;
		current_insertion_mode = InsertionMode::InHead;
	}
	else if ( (token.type == TokenType::StartTag and token.value == "head") or token.type == TokenType::EndTag )
	{
		std::cerr << "Parsing error: Head Start Tag or End tag in after_head mode" << std::endl;
		return;
	}
	else if ( token.type == TokenType::EndTag and (token.value == "body" or token.value == "html" or token.value == "br") )
	{
		HTMLBodyElement* body_element = new HTMLBodyElement(document.html);
		document.html->insert_child(body_element);
		document.body = body_element;

		open_elements.push(body_element);

		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
	else
	{
		HTMLBodyElement* body_element = new HTMLBodyElement(document.html);
		document.html->insert_child(body_element);
		document.body = body_element;

		open_elements.push(body_element);

		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
}

void Parser::in_body_mode()
{
	Token token = m_tokenizer.current();

	if (token.type == TokenType::Character and token.value.empty())
	{
		std::cerr << "Parsing error: NULL character in in_body mode" << std::endl;
		return;
	}
	//else if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
	else if (token.type == TokenType::Character)
	{
		HTMLElement* insert_element = open_elements.top();
		std::cout << "Insert element in character in_body: " << insert_element->element_value << " " << insert_element << std::endl;
		insert_character(token, insert_element);
	}
	else if (token.type == TokenType::Comment)
		return;
	else if (token.type == TokenType::DOCTYPE)
	{
		std::cerr << "Parsing error: DOCTYPE token in in_body mode" << std::endl;
		return;
	}
	else if (token.type == TokenType::StartTag and token.value == "html")
	{
		std::cerr << "Parsing error: html start token in in_body mode" << std::endl;
		return;
	}
	// TODO: base, basefront, bgsouns, ... and template
	else if (token.type == TokenType::StartTag and token.value == "body")
	{
		std::cerr << "Parsing error: body start token in in_body mode" << std::endl;

		HTMLElement* tmp_element = open_elements.top();
		open_elements.pop();
		open_elements.push(tmp_element);

		if (open_elements.top()->element_value != "body" or open_elements.size() == 1)
		{
			return;
		}

		// Checks if the attributes in the token already exist in the body element
		// if one of them is not, add it to the body element
		for (Attr& attribute : token.attributes)
		{
			if (not document.body->contains_attribute(attribute.first))
				document.body->set_attribute(attribute.first, attribute.second);
		}
	}
	// TODO: Frameset
	else if (token.type == TokenType::EndTag and token.value == "body")
	{
		if (open_elements.top()->element_value != "body")
		{
			std::cerr << "Parsing error: top of open elements is not a body element, in in_body mode" << std::endl;
			return;
		}

		// TODO: stack of open elements: dt, li, optgroup...
		open_elements.pop();
		current_insertion_mode = InsertionMode::AfterBody;
	}
	else if (token.type == TokenType::EndTag and token.value == "html")
	{
		if (open_elements.top()->element_value != "body")
		{
			std::cerr << "Parsing error: top of open elements is not a body element, in in_body mode" << std::endl;
			return;
		}

		// TODO: stack of open elements: dt, li, optgroup...

		reconsume_token = true;
		current_insertion_mode = InsertionMode::AfterBody;
	}
	else if (token.type == TokenType::StartTag)
	{
		HTMLElement* new_element = new HTMLElement(token, nullptr);
		HTMLElement* insert_element = open_elements.top();

		insert_element->insert_child(new_element);
		open_elements.push(new_element);
	}

	else if (token.type == TokenType::EndTag and open_elements.top()->element_value == token.value)
	{
		open_elements.pop();
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
		std::cout << "(" << m_tokenizer.current().value << ") Number of open elements: " << open_elements.size() << std::endl;
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
			case InsertionMode::InBody:
				in_body_mode();
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
  print_html_element(document.html, 0);
}
