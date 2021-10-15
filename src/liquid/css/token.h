#pragma once

#include <iostream>

namespace liquid {

enum CSSTokenType
{
  Selector,
  SelectorOption,
  BlockStart,
  BlockEnd,
  Property,
  Value,
  Colon,
  Semicolon
};

struct CSSToken
{
  CSSTokenType type;
  std::string value;
};

std::string css_token_type_as_string(const CSSTokenType type);

}
