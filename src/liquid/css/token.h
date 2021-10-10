#pragma once

#include <iostream>

namespace liquid {

enum CSSTokenType
{
  Whitespace,
  Ident,
  Function,
  AtKeyword,
  Hash,
  String,
  Url,
  Number,
  Dimension,
  Percentage,
  CDO,
  CDC
};

struct CSSToken
{
  CSSTokenType type;
  std::string type_flag;

  std::string value;
};

}
