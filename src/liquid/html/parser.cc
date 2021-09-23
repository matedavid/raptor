#include "parser.h"

void Parser::initial_mode()
{
  Token token = m_tokenizer.current();

  if ( token.type == TokenType::Character and (token.value == "\t" or token.value == "\n" or token.value == "\f" or token.value == " ") )
    return;
  else if (token.type == TokenType::Comment)
  {
    
  }
  else if (token.type == TokenType::DOCTYPE)
  {

  }
}

void Parser::before_html_mode()
{

}

Parser::Parser()
{
}

void Parser::parse(Tokenizer& tokenizer)
{
  m_tokenizer = tokenizer;
  current_insertion_mode = InsertionMode::Initial;

  // document = Document();

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

    m_tokenizer.next();
  }
}
