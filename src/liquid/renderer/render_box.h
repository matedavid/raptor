#pragma once

#include <iostream>
#include <vector>

#include "liquid/html/html_element.h"

namespace liquid {

enum RenderBoxType
{
  Default,
  Txt,
  Img
};

enum RenderBoxDisplay
{
  None,
  Block,
  Inline,
  ListItem,
  // TODO: Missing values...
};

enum RenderBoxPosition
{
  Static,
  Relative,
  Absolute,
  Fixed,
  Sticky,
};

class RenderBox
{
private:
  // RenderBoxType m_type = RenderBoxType::Default;
  // bool m_printable     = false;

  // display css property
  RenderBoxDisplay  display;
  // position css property
  RenderBoxPosition position;

  // The HTML element that the RenderBox encapsulates  
  HTMLElement* node;
  // The parent of the current RenderBox
  RenderBox* parent;

  // RenderBox children 
  std::vector<RenderBox*> children;

public:
  virtual RenderBoxType type() const { return RenderBoxType::Default; }
  virtual bool is_printable()  const { return false; }
  
  RenderBox(HTMLElement* node, RenderBox* parent);

  // Computes the height of the RenderBox
  void compute_height();

  // Lays out the RenderBox on the DOM
  void layout();

  // DEBUG
  virtual void print(int n_tabs);
};

}
