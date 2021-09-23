#pragma once

#include <iostream>

#include "node.h"

class CharacterData : public Node 
{
public:
  CharacterData();

  std::string data();
  uint length();

  std::string substring_data(uint offset, uint count);
  void append_data(std::string data);
  void insert_data(uint offset, std::string data);
  void delete_data(uint offset, uint count);
  void replace_data(uint offset, uint count, std::string data);
};
