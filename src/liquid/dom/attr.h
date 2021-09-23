#pragma once

#include <iostream>

class Element;

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
