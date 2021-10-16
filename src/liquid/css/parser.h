#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "liquid/css/tokenizer.h"



namespace liquid {

/* ===== DIFFERENT FILE? ===== */
#define CSSClassModifier "."
#define CSSIDModifier "#"

enum SelectorRelation
{
  Grouping,
  Descendant
};

enum SelectorValueOption
{
  Type,
  Class,
  ID
};

struct SelectorValue
{
  SelectorValueOption option;

  std::string value;
  std::string option_value;
};

struct CSSBlock
{
  SelectorRelation selector_relation;
  std::vector<SelectorValue> selector_targets;

  std::map< std::string, std::vector<std::string> > declarations;
};
/* =========================== */

class CSSParser
{
private:
  CSSTokenizer m_tokenizer;

  // Vector of Parsed blocks
  std::vector<CSSBlock> m_blocks;

  void parse_selector(CSSBlock& block);
  void parse_block(CSSBlock& block);
  void parse_declaration(CSSBlock& block);

public:
  CSSParser();

  void parse(CSSTokenizer& tokenizer);
};

}