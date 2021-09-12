#pragma once

#include <iostream>

#include "liquid/dom/element.h"

class HTMLElement : public Element
{

public:
  std::string title;
  std::string lang;
  bool translate;
  std::string dir;

  bool hidden;
  std::string access_key;
  std::string access_key_label;
  bool draggable;
  bool spell_check;
  std::string auto_capitalize;

  std::string inner_text;
  std::string outer_text;

public:
  HTMLElement();

  void click();
};
