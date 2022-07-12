#pragma once

#include <iostream>
#include <vector>

#include "liquid/html/html_element.h"

namespace liquid {

// TODO: Shouldn't be here
struct Dimensions
{
  float width;
  float height;
};
// // // 

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
  struct Line
  {
    bool horizontal;
    std::vector<RenderBox*> elements;

    float height;
  };

  // Constructs a line representation of the RenderBox's children.
  // A line representation lays out elements vertically or horizontally, as they would be displayed
  // in screen.
  // 
  // For example, Block elements would consist of independent lines (as they line break):
  //  ___________
  // |   Block   |
  //  ‾‾‾‾‾‾‾‾‾‾‾
  //  ___________
  // |   Block   |
  //  ‾‾‾‾‾‾‾‾‾‾‾
  // And inline elements would populate a single line:
  //  ____________________________________
  // |   Inline   |   Inline   |   ...    |
  //  ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
  // 
  // This representation helps laying out elements, as it provides a clear representation of 
  // elements that should be positioned horizontally and elements that should be positioned vertically.
  std::vector<Line> layout_lines() const;

protected:
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

  // Margin values
  EdgeValues<float> margin;
  // Padding values
  EdgeValues<float> padding;
  // Border width values
  EdgeValues<float> border_width;

  // position in DOM
  float x, y;

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
  // NOTE: Idem margin_height
  float margin_width;

public:
  virtual RenderBoxType type() const { return RenderBoxType::Default; }
  virtual bool is_printable()  const { return false; }
  
  RenderBox(HTMLElement* node, RenderBox* parent);

  // Computes the height of the RenderBox
  virtual Dimensions compute_dimensions(float container_width);

  // Computes x & y position of the RenderBox
  void layout();

  void insert_child(RenderBox* child);

  // DEBUG
  virtual void print(int n_tabs);
};

}
