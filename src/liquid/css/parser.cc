#include "parser.h"

namespace liquid {

void CSSParser::parse_selector(CSSBlock& block)
{
  CSSToken token = m_tokenizer.current();
  while (token.type != CSSTokenType::BlockStart)
  {
    SelectorValue selector_value;

    // TODO: This should be changed, as more SelectorRelation's are added
    if (token.type == CSSTokenType::SelectorOption and (token.value == " " or token.value == ","))
    {
      if (token.value == " ")
        block.selector_relation = SelectorRelation::Descendant;
      else if (token.value == ",")
        block.selector_relation = SelectorRelation::Grouping;
    }
    else if (token.type == CSSTokenType::SelectorOption)
    {
      selector_value.option = token.value == CSSClassModifier ? SelectorValueOption::Class : SelectorValueOption::ID;
      selector_value.option_value = token.value;
      token = m_tokenizer.next();

      if (token.type != CSSTokenType::Selector)
      {
        std::cout << "Parsing error: No selector after SelectorOption" << std::endl;
        continue;
      }
      else
      {
        selector_value.value = token.value;
        block.selector_targets.push_back(selector_value);
      }
    }
    else if (token.type == CSSTokenType::Selector)
    {
      selector_value.option = SelectorValueOption::Type;
      selector_value.value = token.value;
      block.selector_targets.push_back(selector_value);
    }

    token = m_tokenizer.next();
  }

  // Jump the BlockStart Token
  m_tokenizer.next();
  parse_block(block);
}

void CSSParser::parse_block(CSSBlock& block)
{
  CSSToken token = m_tokenizer.current();
  while (token.type != CSSTokenType::BlockEnd)
  {
    if (token.type == CSSTokenType::DeclarationStart)
    {
      // Jump the DeclarationStart token
      m_tokenizer.next();
      parse_declaration(block);
      token = m_tokenizer.current();
    }
    else
    {
      token = m_tokenizer.next();
    }
  }
}

void CSSParser::parse_declaration(CSSBlock& block)
{
  std::pair<std::string, std::vector<std::string>> declaration;
  CSSToken token;

  token = m_tokenizer.current();
  declaration.first = token.value;

  // Jump the ':' token
  m_tokenizer.next();
  token = m_tokenizer.current();
  while (token.type != CSSTokenType::DeclarationEnd)
  {
    declaration.second.push_back(token.value);
    token = m_tokenizer.next();
  }

  block.declarations.insert(declaration);

  // Jump the DeclarationEnd token
  m_tokenizer.next();
}

CSSParser::CSSParser()
{
}

void CSSParser::parse(CSSTokenizer& tokenizer)
{
  m_tokenizer = tokenizer;

  while (not m_tokenizer.is_last())
  {
    CSSBlock block;
    parse_selector(block);

    m_blocks.push_back(block);
  }
}

}