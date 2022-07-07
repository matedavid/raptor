#pragma once

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
  RenderBoxType type;

  // display css property
  RenderBoxDisplay  display;
  // position css property
  RenderBoxPosition position;

  HTMLElement* node;

public:
  virtual bool is_printable()  const { return false; }
  virtual RenderBoxType type() const { return RenderBoxType::Default; }
  
  RenderBox(HTMLElement* node);

  // Computes the height of the RenderBox
  void compute_height();

  // Lays out the RenderBox on the DOM
  void layout();
};

}
