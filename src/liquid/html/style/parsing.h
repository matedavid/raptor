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

// Parses number used in CSS from string
CSSNumber parse_number(const std::string& value);

// Returns if value is a CSSNumber
bool is_value_css_number(const std::string& value);

//std::pair<CSSNumber, CSSNumber> parse_pair_numbers(const std::vector<std::string>& value, int i);

// Returns if the property is an inherited property
bool is_property_inherited(const std::string& property);

// Gets the float value from a CSSNumber
float get_css_number(CSSNumber& n, float& em_reference_value);

// DEBUG
void print_style(const HTMLElementCSSValues& style);