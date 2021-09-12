#pragma once

#include <iostream>

#include "character_data.h"

class Comment : public CharacterData
{
public:
  Comment();
  Comment(const std::string& data);
};