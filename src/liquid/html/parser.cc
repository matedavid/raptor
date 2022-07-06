#include "parser.h"

namespace liquid {

void HTMLParser::insert_character(const Token& token, HTMLElement* element)
{
	// Check if the element has a Text element as the "last child". 
	// If it has, append the string to the already existing text element
	// If not, create a new Text child 
	std::vector<HTMLElement*> children = element->child_elements();

	HTMLElement* text_element = nullptr;
	if (children.size() != 0 and children[children.size()-1]->type() == HTMLElementType::TextType)
		text_element = children[children.size()-1];

	if (text_element != nullptr and text_element->type() == HTMLElementType::TextType)
	{
		Text* text = dynamic_cast<Text*>(text_element);
		if (text == nullptr)
			return;
		text->append_string(token.value);
	}
	else
	{
		Text* text = new Text(token.value);;
		element->insert_child(text);
	}
}

// ===== Parsing modes =====
void HTMLParser::text_mode()
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

void HTMLParser::initial_mode()
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

void HTMLParser::before_html_mode()
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
		HTMLHtmlElement* html_element = new HTMLHtmlElement(token, nullptr, m_document_path);
		html = html_element;
		open_elements.push(html_element);

		current_insertion_mode = InsertionMode::BeforeHead;
	}
	else if (token.type == TokenType::EndTag)
		std::cerr << "Parsing error: Before html end tag" << std::endl;
	else if ( token.type == TokenType::EndTag and (token.value == "head" or token.value == "body" or token.value == "html" or token.value == "br") )
	{
		HTMLHtmlElement* html_element = new HTMLHtmlElement(nullptr);
		html = html_element;
		open_elements.push(html_element);

		reconsume_token = true;
		current_insertion_mode = InsertionMode::BeforeHead;
	}
	else
	{
		HTMLHtmlElement* html_element = new HTMLHtmlElement(nullptr);
		html = html_element;
		open_elements.push(html_element);

		reconsume_token = true;
		current_insertion_mode = InsertionMode::BeforeHead;
	}
}

void HTMLParser::before_head_mode()
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
		HTMLHeadElement* head_element = new HTMLHeadElement(token, html, m_document_path);
		html->insert_child(head_element);

    open_elements.push(head_element);

		//document.head = head_element;
		current_insertion_mode = InsertionMode::InHead;
	}
	else if (token.type == TokenType::EndTag)
	{
		std::cerr << "Parsing error: end tag in before_head mode" << std::endl;
		return;
	}
	else
	{
		HTMLHeadElement* head_element = new HTMLHeadElement(html);
		html->insert_child(head_element);

    open_elements.push(head_element);

		//document.head = head_element;
		reconsume_token = true;
		current_insertion_mode = InsertionMode::InHead;
	}
}

void HTMLParser::in_head_mode()
{
  Token token = m_tokenizer.current();

	if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
  {
		//HTMLElement* insert_element = open_elements.top();
		//insert_character(token, insert_element);
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
		HTMLElement* element = new HTMLElement(token, nullptr, m_document_path);
		element->element_value = token.value;

		HTMLElement* insert_element = open_elements.top();
		insert_element->insert_child(element);

		if (not token.self_closing)
			open_elements.push(element);

  }
  else if (token.type == TokenType::StartTag and token.value == "meta")
  {
		// Every meta tag is self-closing, even if "token.self_closing" is false (doesn't finish with />)
		HTMLElement* meta_element = new HTMLElement(token, open_elements.top(), m_document_path);
		open_elements.top()->insert_child(meta_element);
  }
  else if (token.type == TokenType::StartTag and token.value == "title")
  {
    HTMLTitleElement* title_element = new HTMLTitleElement(token, nullptr, m_document_path);
		//document.head->insert_child(title_element);
		HTMLElement* insert_element = open_elements.top();
		insert_element->insert_child(title_element);

		open_elements.push(title_element);

    original_insertion_mode = current_insertion_mode;
    current_insertion_mode = InsertionMode::TextMode;
  }
	else if (token.type == TokenType::StartTag and token.value == "style")
	{
		HTMLElement* style_element = new HTMLElement(token, open_elements.top(), m_document_path);
		open_elements.push(style_element);

		original_insertion_mode = current_insertion_mode;
		current_insertion_mode = InsertionMode::InStyle;
	}
	// TODO: Missing multple parts, not implementing at the moment
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

void HTMLParser::in_style_mode()
{
	Token token = m_tokenizer.current();
	if (token.type == TokenType::EndTag && token.value == "style")
	{
		HTMLElement* script_element = open_elements.top();
		open_elements.pop();
		open_elements.top()->insert_child(script_element);

		current_insertion_mode = original_insertion_mode;
	}
	else 
	{
		HTMLElement* style_element = open_elements.top();
		insert_character(token, style_element);
	}
}

void HTMLParser::after_head_mode()
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
		HTMLBodyElement* body_element = new HTMLBodyElement(token, html, m_document_path);
		html->insert_child(body_element);
		//document.body = body_element;

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
		HTMLBodyElement* body_element = new HTMLBodyElement(html);
		html->insert_child(body_element);
		//document.body = body_element;

		open_elements.push(body_element);

		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
	else
	{
		HTMLBodyElement* body_element = new HTMLBodyElement(html);
		html->insert_child(body_element);
		// document.body = body_element;

		open_elements.push(body_element);

		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
}

void HTMLParser::in_body_mode()
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
			// NOTE: Not sure this is a good idea (no checking that it actually exists...)
			HTMLElement* body = html->get_elements_by_tag_name("body")[0];
			if (not body->contains_attribute(attribute.first))
				body->set_attribute(attribute.first, attribute.second);
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
		/*
			HTMLElement* new_element = new HTMLElement(token, nullptr, m_document_path);
		*/
		HTMLElement* new_element;
		if (token.value == "div")
			new_element = new HTMLDivElement(token, nullptr, m_document_path);
		else if (token.value == "p")
			new_element = new HTMLParagraphElement(token, nullptr, m_document_path);
		else if (token.value == "span")
			new_element = new HTMLSpanElement(token, nullptr, m_document_path);
		else if (token.value == "h1" or
						 token.value == "h2" or
						 token.value == "h3" or
						 token.value == "h4" or
						 token.value == "h5" or
						 token.value == "h6") 
		{
			HTMLHeadingElement* heading = new HTMLHeadingElement(token, nullptr, m_document_path);
			switch (token.value[1])
			{
				case '1':
					heading->level = 1; break;
				case '2':
					heading->level = 2; break;
				case '3':
					heading->level = 3; break;
				case '4': 
					heading->level = 4; break;
				case '5':
					heading->level = 5; break;
				case '6':
					heading->level = 6; break;
			}
			new_element = heading;
		} 
		else if (token.value == "em")
			new_element = new HTMLEmphasisElement(token, nullptr, m_document_path);
		else if (token.value == "i")
			new_element = new HTMLItalicizedElement(token, nullptr, m_document_path);
		else if (token.value == "strong")
			new_element = new HTMLStrongElement(token, nullptr, m_document_path);
		else if (token.value == "a")
			new_element = new HTMLAnchorElement(token, nullptr, m_document_path);
		else if (token.value == "ol")
			new_element = new HTMLOrderedListElement(token, nullptr, m_document_path);
		else if (token.value == "ul")
			new_element = new HTMLUnorderedListElement(token, nullptr, m_document_path);
		else if (token.value == "li")
			new_element = new HTMLListItemElement(token, nullptr, m_document_path);
		else if (token.value == "img")
			new_element = new HTMLImageElement(token, nullptr, m_document_path);
		else 
			new_element = new HTMLElement(token, nullptr, m_document_path);

		HTMLElement* insert_element = open_elements.top();
		insert_element->insert_child(new_element);

		if (not token.self_closing)
			open_elements.push(new_element);
	}

	else if (token.type == TokenType::EndTag and open_elements.top()->element_value == token.value)
	{
		open_elements.pop();
	}
}

void HTMLParser::after_body_mode()
{
	Token token = m_tokenizer.current();

	if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
	{
		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
	else if (token.type == TokenType::Comment)
		return;
	else if (token.type == TokenType::DOCTYPE)
	{
		std::cerr << "Parsing error: DOCTYPE token in after_body mode" << std::endl;
		return;
	}
	else if (token.type == TokenType::StartTag and token.value == "html")
	{
		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
	else if (token.type == TokenType::EndTag and token.value == "html")
	{
		current_insertion_mode = InsertionMode::AfterAfterBody;
	}
	else
	{
		std::cerr << "Parsing error: else clause reached in after_body mode" << std::endl;

		reconsume_token = true;
		current_insertion_mode = InsertionMode::InBody;
	}
}

HTMLParser::HTMLParser()
{
}

// === TEMPORAL ===
const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s)
{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
		return rtrim(ltrim(s));
}
// ================

HTMLHtmlElement* HTMLParser::parse(Tokenizer& tokenizer, const std::string& document_path)
{
	m_document_path = std::filesystem::path(document_path);
	m_document_path.remove_filename();

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
			case InsertionMode::InStyle:
				in_style_mode();
				break;
      case InsertionMode::AfterHead:
        after_head_mode();
        break;
			case InsertionMode::InBody:
				in_body_mode();
				break;
			case InsertionMode::AfterBody:
				after_body_mode();
      // ...
      default:
        std::cout << "Insertion Mode Unknown" << std::endl;
    }

		if (not reconsume_token)
			m_tokenizer.next();
		else
			reconsume_token = false;
  }

	// Removes text elements which have empty strings
	std::vector<HTMLElement*> text_elements = html->get_elements_by_tag_name("text");
	for (HTMLElement* element : text_elements)
	{
		Text* text_element = dynamic_cast<Text*>(element);
		if (text_element == nullptr) continue;

		std::string trimmed_string = trim(text_element->content());
		if (trimmed_string.empty())
			html->remove_child(text_element);
	}

	/*
	std::cout << "Printing AST result from parsing:" << std::endl;
  print_html_element(document.html, 0);
	*/

	return html;
}

}
