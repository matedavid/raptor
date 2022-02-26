#pragma once

#include "liquid/html/html_element.h"

namespace liquid {

enum RenderBoxType
{
  Block,
  Inline
};

template<typename T> 
struct EdgeValues
{
  T top;
  T right;
  T bottom;
  T left;
};

class RenderBox
{
private:
  RenderBoxType type;

  float x, y;
  float width, height;

  EdgeValues<float> margin;
  EdgeValues<float> border;
  EdgeValues<float> padding;

  HTMLElement* node;

  RenderBox* parent;
  std::vector<RenderBox*> children;

public: 
  virtual bool is_printable() { return false; }

public:
  RenderBox();
  RenderBox(const HTMLElement* element);

  void layout(const HTMLElement* element);

  float get_x() { return x; }
  float get_y() { return y; }
};

}