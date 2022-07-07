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
  // display css property
  RenderBoxDisplay  display;
  // position css property
  RenderBoxPosition position;

protected:
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
  float compute_height();

  // Lays out the RenderBox on the DOM
  void layout();

  void insert_child(RenderBox* child);

  // DEBUG
  virtual void print(int n_tabs);
};

}
