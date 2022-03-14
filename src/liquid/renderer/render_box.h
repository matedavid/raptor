#pragma once

#include "liquid/html/html_element.h"
#include "liquid/html/text.h"

namespace liquid {

enum RenderBoxDisplayType
{
  None,
  Block,
  Inline
};

enum RenderBoxType 
{
  Default,
  Txt,
  Img 
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

  RenderBox* parent;
  std::vector<RenderBox*> children;

  float x, y;
  // box_width/height is all of the space taken up by the RenderBox (+padding +border +margin)
  float box_width = -1, box_height = -1;
  // content_width/height is only the space taken up by the content area box
  float content_width = -1, content_height = -1;

public: 
  // TEMPORARY?
  EdgeValues<float> margin;
  EdgeValues<float> padding;
  EdgeValues<float> content;

public: 
  HTMLElement* node;

  virtual bool is_printable() const { return false; }
  virtual RenderBoxType type() const { return RenderBoxType::Default; }

private:
  float resolve_border_width(const std::string& border_width_value) const;

public:
  RenderBox();
  RenderBox(HTMLElement* element, RenderBox* parent);

  // Computes the (x,y) reference position 
  std::pair<float, float> compute_xy_reference();
  // Computes the layout of the object 
  virtual void layout(float container_width);
  // Method for reflow
  void reflow(float upstream_width);
  // Computes height from accumulated_height and all remaining margin/padding/border values
  void compute_height(float accumulated_height);


  RenderBoxDisplayType get_display_type() const { return display_type; }

  void set_x(float _x) { x = _x;; }
  void set_y(float _y) { y = _y; }

  float get_x() const { return x; }
  float get_y() const { return y; }
  
  // Creating different function for ref_y because, when using get_y() as a reference point, the return value
  // has already taken into account the margin, border and padding values, so it's not a good reference value for other
  // boxes (for example, when we have two inline tags)
  float get_ref_y() const;

  //void set_width(float _width)   { width = _width; }
  //void set_height(float _height) { height = _height; }

  float get_width()  const { return box_width;  }
  float get_height() const { return box_height; }

  float get_content_width()  const { return content_width; }
  float get_content_height() const { return content_height; }

  void add_child(RenderBox* child) { children.push_back(child); }
  std::vector<RenderBox*> get_children() const { return children; }

  void set_parent(RenderBox* _parent) { parent = _parent; }
  RenderBox* get_parent() const { return parent; }

  void print(int number_tabs);
};

}
