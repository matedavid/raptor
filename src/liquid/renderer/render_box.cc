#include "render_box.h"

namespace liquid 
{

bool is_number(const std::string& value)
{
  return value.find_first_not_of("0123456789.") == std::string::npos;
}

float RenderBox::resolve_border_width(const std::string& border_width_value)
{
  if (border_width_value == "medium")
    return 3.0;
  else if (border_width_value == "thin")
    return 1.0;
  else if (border_width_value == "thick")
    return 5.0;

  if (is_number(border_width_value))
    return std::stof(border_width_value);

  return 0.;
}

RenderBox::RenderBox()
{
}

RenderBox::RenderBox(HTMLElement* element, RenderBox* parent)
  : node(element), parent(parent)
{
}

void RenderBox::layout(uint _width)
{
  // Specify and set display_type
  if (node->style.display == "none")
    display_type = RenderBoxDisplayType::None;
  else if (node->style.display == "block")
    display_type = RenderBoxDisplayType::Block;
  else if (node->style.display == "inline")
    display_type = RenderBoxDisplayType::Inline;

  // Compute border-width values
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.;
  float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.;
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.;

  // General workflow:
  // 1. Determine width and (x,y)
  width = _width - node->style.margin_left - node->style.margin_right;

  // Find (x,y) reference point to compute RenderBox (x,y) values
  float xref, yref;
  if (parent == nullptr)
  {
    xref = yref = 0;
  }
  else
  {
    std::vector<RenderBox*> sibilings = parent->children;
    int number_sibilings = sibilings.size();

    if (number_sibilings >= 1)
    {
      RenderBox* last_sibiling = sibilings[sibilings.size()-1];
      xref = last_sibiling->get_x();
      yref = last_sibiling->get_y()+last_sibiling->get_height()+last_sibiling->node->style.margin_bottom;
    }
    else 
    {
      xref = parent->get_x();
      yref = parent->get_y();
    }
  }

  x = xref + node->style.margin_left + node->style.padding_left + border_left_value;
  y = yref + node->style.margin_top + node->style.padding_top + border_top_value;

  // Compute padding, border and margin edges
  padding.top = y - node->style.padding_top;
  padding.right = x + width + node->style.padding_right;
  padding.left = x - node->style.padding_left;

  border.top = padding.top - border_top_value;
  border.right = padding.right + border_right_value;
  border.left = padding.left - border_left_value;

  margin.top = border.top - node->style.margin_top;
  margin.right = border.right + node->style.margin_right;
  margin.left = border.left - node->style.margin_left;

  // margin-top collapsing
  if (parent != nullptr and parent->children.size() >= 1)
  {
    RenderBox* sibiling = parent->children[parent->children.size()-1];
    float max_margin = std::max<float>(node->style.margin_top, sibiling->node->style.margin_bottom);
    float difference = (node->style.margin_top + sibiling->node->style.margin_bottom) - max_margin;

    y -= difference;
    margin.top -= difference;
    border.top -= difference;
    padding.top -= difference;
  }
}

void RenderBox::compute_height(float accumulated_height)
{
  // Bottom border-width value
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.;

  // Compute padding border and margin bottom edge
  padding.bottom = y + accumulated_height + node->style.padding_bottom;
  border.bottom = padding.bottom + border_bottom_value;
  margin.bottom  = border.bottom + node->style.margin_bottom;

  height = accumulated_height + node->style.padding_bottom + border_bottom_value;
}

void RenderBox::print(int number_tabs)
{
  for (int i = 0; i < number_tabs; ++i)
  {
    std::cout << "  ";
  }

  std::cout << "(" << node->element_value << ") RenderBox: (" << x << "," << y << ") width=" << width << " height="  << height << 
    " margin=(" << margin.top << " " << margin.right << " " << margin.bottom << " " << margin.left << ")" << 
    " padding=(" << padding.top << " " << padding.right << " " << padding.bottom << " " << padding.left << ")" << std::endl;

  for (auto child : children)
  {
    child->print(number_tabs+1);
  }
}

}
