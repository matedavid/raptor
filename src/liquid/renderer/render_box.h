#pragma once

#include <iostream>
#include <vector>

#include "liquid/html/html_element.h"

#include <SFML/Graphics/Text.hpp>

namespace liquid {

// TODO: Shouldn't be here
struct AppliedDimensions
{
  bool width  = false;
  bool height = false;
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
  InlineBlock,
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
protected:
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
  struct Line
  {
    bool horizontal;
    std::vector<RenderBox*> elements;
  };

  // Constructs a line representation of the RenderBox's children
  void layout_lines(std::vector<Line>& lines, std::vector<RenderBox*>& not_in_flow) const;

  // Input struct for Layout function
  struct LayoutParameters
  {
    float xref, yref;
    float margin_top_applied = 0.f;
    float container_width;
  };

  // Result struct from Layout function 
  struct LayoutResult
  {
    // Used to communicate to parent elements how much margin top 
    // they should apply
    float margin_top_remaining = 0.f;

    // The resulting margin top/bottom of a container
    float resulting_margin_top = 0.f;
    float resulting_margin_bottom = 0.f;
  };

  // display css property
  RenderBoxDisplay  display;
  // position css property
  RenderBoxPosition position;
  // If the element is positioned according to the normal document flow
  bool in_document_flow;

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
  float content_height = 0.f;
  // Height of the content box + padding + border_width
  float height = 0.f;
  
  // Width of the content box
  float content_width = 0.f;
  // Width of the content box + padding + border_width
  float width = 0.f;

private:
  // Returns if child is the first child element in_document_flow
  bool is_first_child(RenderBox* child)
  {
    for (auto c : children)
    {
      if (c->in_document_flow)
        return child == c;
    }
    return false;
  }

public:
  virtual RenderBoxType type() const { return RenderBoxType::Default; }
  virtual bool is_printable()  const { return false; }
  
  RenderBox(HTMLElement* node, RenderBox* parent);

  // Computes the height of the RenderBox
  virtual AppliedDimensions compute_dimensions(float container_width);

  // Computes x & y position of the RenderBox
  virtual LayoutResult layout(LayoutParameters params);

  void insert_child(RenderBox* child);

  std::vector<RenderBox*> get_children() const { return children; }

  float get_x() { return x; }
  float get_y() { return y; }

  // DEBUG
  virtual void print(int n_tabs);
};

}
