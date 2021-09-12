#pragma once

#include <iostream>
#include <vector>

class Element;

class HTMLCollection
{
private:
  std::vector<Element*> m_elements;

public:
  HTMLCollection();

  uint length();
  Element* item(uint index);
  Element* named_item(const std::string& name);
};
