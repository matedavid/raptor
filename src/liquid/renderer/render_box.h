#pragma once

#include "utils.h"
#include "liquid/html/html_element.h"
#include "liquid/html/text.h"

#include <SFML/Graphics/Text.hpp>

namespace liquid {

enum RenderBoxDisplayType
{
  None,
  Block,
  Inline,
  ListItem
};

enum RenderBoxType 
{
  Default,
  Txt,
  Img 
};

enum RenderBoxPosition 
{
  Static,
  Relative,
  Absolute,
  Fixed,
  Sticky
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
protected:
  RenderBoxDisplayType display_type;
  RenderBoxPosition position;

  // Specifies if the render_box is in the normal document flow
  bool in_flow = true;

  RenderBox* parent;
  std::vector<RenderBox*> children;

  float x, y;
  float xref, yref;
  // content_width/height is only the space taken up by the content area box
  float content_width = -1, content_height = -1;

public:
  HTMLElement* node;

  virtual bool is_printable() const { return false; }
  virtual RenderBoxType type() const { return RenderBoxType::Default; }
  
  bool parent_first_child_mc = false;
  bool parent_last_child_mc = false;
  bool adjacent_sibling_mc = false;

public:
  RenderBox();
  RenderBox(HTMLElement* element, RenderBox* parent);

  // Computes the (x,y) reference position 
  std::pair<float, float> compute_xy_reference() const;
  // Computes the layout of the object 
  virtual void layout(float container_width);
  // Method for reflow
  void reflow(float upstream_width);
  // Computes height from accumulated_height and all remaining margin/padding/border values
  void compute_height(float accumulated_height);

  RenderBoxDisplayType get_display_type() const { return display_type; }
  RenderBoxPosition get_position_type()   const { return position; }

  bool in_document_flow() const { return in_flow; }

  float get_x() const { return x; }
  float get_y() const { return y; }

  // Gets the x/y value including the padding and the border
  float get_border_x() const;
  float get_border_y() const;
  
  // Creating different function for ref_y because, when using get_y() as a reference point, the return value
  // has already taken into account the margin, border and padding values, so it's not a good reference value for other
  // boxes (for example, when we have two inline tags)
  float get_ref_y() const;

  // box_width/height is all the space taken up by the RenderBox (+padding +border +margin)
  float get_box_width()  const;
  float get_box_height() const;

  // Returns width/height containing content + padding + border
  float get_width() const;
  float get_height() const;

  float get_content_width()  const { return content_width; }
  float get_content_height() const { return content_height; }

  std::vector<std::pair<float, float>> get_border_edges() const;

  // This method is mainly used to compute the bottom sibling's y position value
  float get_vertical_separation() const;

  void add_child(RenderBox* child) { children.push_back(child); }
  std::vector<RenderBox*> get_children() const { return children; }

  void set_parent(RenderBox* _parent) { parent = _parent; }
  RenderBox* get_parent() const { return parent; }

  void print(int number_tabs);
};

}
