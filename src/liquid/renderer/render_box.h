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
  float width, height;

public: // TEMPORARY?
  EdgeValues<float> margin;
  EdgeValues<float> border;
  EdgeValues<float> padding;

public: 
  HTMLElement* node;

  virtual bool is_printable() const { return false; }
  virtual RenderBoxType type() const { return RenderBoxType::Default; }

private:
  float resolve_border_width(const std::string& border_width_value);

public:
  RenderBox();
  RenderBox(HTMLElement* element, RenderBox* parent);

  // Computes width, (x,y) position, margin/padding/border and display type
  void layout(uint _width);
  // Computes height from accumulated_height and all remaining margin/padding/border values
  void compute_height(float accumulated_height);

  RenderBoxDisplayType get_display_type() const { return display_type; }

  void set_x(float _x) { x = _x;; }
  void set_y(float _y) { y = _y; }

  float get_x() const { return x; }
  float get_y() const { return y; }

  void set_width(float _width)   { width = _width; }
  void set_height(float _height) { height = _height; }

  float get_width()  const { return width;  }
  float get_height() const { return height; }

  void add_child(RenderBox* child) { children.push_back(child); }
  std::vector<RenderBox*> get_children() const { return children; }

  void set_parent(RenderBox* _parent) { parent = _parent; }
  RenderBox* get_parent() const { return parent; }

  void print(int number_tabs);
};

}
