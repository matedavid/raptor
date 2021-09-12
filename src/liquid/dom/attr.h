#pragma once

#include <iostream>

#include "element.h"

class Attr
{
private:
  std::string prefix;
  std::string local_name;
  std::string name;
  std::string value;

  Element* owner_element;

public:
  Attr();
};
