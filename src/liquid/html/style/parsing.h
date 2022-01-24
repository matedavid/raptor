#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <regex>

#include "html_element_css_values.h"

enum CSSNumberType
{
  Px,
  Percentage,
  Em,
  Error
};

struct CSSNumber 
{
  float value;
  CSSNumberType type;
};

CSSNumber parse_number(const std::string& value);
std::pair<CSSNumber, CSSNumber> parse_pair_numbers(const std::vector<std::string>& value, int i);

// DEBUG
void print_style(const HTMLElementCSSValues& style);