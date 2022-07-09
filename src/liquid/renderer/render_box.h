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

struct Dimensions
{
  float width;
  float height;
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

  // Height of the content box
  float content_height;
  // Height of the content box + padding + border_width
  float height;
  // Height of the complete render box (box_height + margin)
  // NOTE: Maybe not necessary, as margin only pushes other elements way instead of contributing to height
  //       could create get_margin() function instead that returns the margin values and then push other elements
  float margin_height;
  
  // Width of the content box
  float content_width;
  // Width of the content box + padding + border_width
  float width;
  // Height of the complete render box (box_height + margin)
  // NOTE: Maybe not necessary, as margin only pushes other elements way instead of contributing to width 
  //       could create get_margin() function instead that returns the margin values and then push other elements
  float margin_width;

public:
  virtual RenderBoxType type() const { return RenderBoxType::Default; }
  virtual bool is_printable()  const { return false; }
  
  RenderBox(HTMLElement* node, RenderBox* parent);

  // Computes the height of the RenderBox
  virtual Dimensions compute_dimensions(float container_width);

  // Lays out the RenderBox on the DOM
  virtual void layout();

  void insert_child(RenderBox* child);

  // DEBUG
  virtual void print(int n_tabs);
};

}
