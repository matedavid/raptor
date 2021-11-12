#include "token.h"

namespace liquid {

std::string css_token_type_as_string(const CSSTokenType type)
{
  switch (type)
  {
    case CSSTokenType::Selector:
      return "Selector";
    case CSSTokenType::SelectorOption:
      return "SelectorOption";
    case BlockStart:
      return "BlockStart";
    case CSSTokenType::BlockEnd:
      return "BlockEnd";
    case CSSTokenType::DeclarationStart:
      return "DeclarationStart";
    case CSSTokenType::DeclarationEnd:
      return "DeclarationEnd";
    case CSSTokenType::Property:
      return "Property";
    case CSSTokenType::Value:
      return "Value";
    case CSSTokenType::Colon:
      return "Colon";
    case CSSTokenType::Semicolon:
      return "Semicolon";
    case CSSTokenType::CSSComment:
      return "Comment";
  }
  
  return "UNKNOWN";
}

}
