#pragma once

#include <iostream>
#include <map>

#include "attr.h"

class NamedNodeMap
{
private:
  std::map<std::string, Attr*> content;

public:
  NamedNodeMap();

  uint length();
  Attr* item(uint index);
  Attr* get_named_item(const std::string& name);
  Attr* set_named_item(Attr attr);
  Attr* remove_named_item(const std::string& name)
};
