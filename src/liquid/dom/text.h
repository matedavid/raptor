#pragma once

#include <iostream>

#include "character_data.h"

class Text : public CharacterData
{
public:
  Text();
  Text(const std::string& data);

  void split_text(uint offset);
  std::string whole_text();
};
