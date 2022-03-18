#include "render_box.h"

namespace liquid 
{

/* ===== UTIL ===== */
bool is_number(const std::string& value)
{
  return value.find_first_not_of("0123456789.") == std::string::npos;
}
/* ===== ==== ===== */

float RenderBox::resolve_border_width(const std::string& border_width_value) const
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

std::pair<float, float> RenderBox::compute_xy_reference()
{
  if (parent == nullptr)
    return { 0, 0 };

  std::vector<RenderBox*> siblings = parent->children;
  if (siblings.size() < 1)
    return { parent->get_x(), parent->get_y() };

  RenderBox* last_sibiling = siblings[siblings.size()-1];
  if (last_sibiling->display_type == RenderBoxDisplayType::Inline and display_type == RenderBoxDisplayType::Inline)
  {
    float xref = last_sibiling->get_x() + last_sibiling->get_width();
    float yref = last_sibiling->get_ref_y();

    std::cout << "Referring to second div: " << last_sibiling->get_width() << std::endl;
    std::cout << xref << " " << yref << std::endl;

    return { xref, yref };
  }

  // If last_sibiling or current render_box has display_type = block, block starts in new line,
  // so yref has to take into account height of last_sibiling
  float xref = last_sibiling->get_x();
  //float yref = last_sibiling->get_y() + last_sibiling->get_height();
  float yref = last_sibiling->get_y() + last_sibiling->get_vertical_separation();

  return { xref, yref };
}

void RenderBox::layout(float container_width)
{
  // Specify and set display_type
  if (node->style.display == "none")
    display_type = RenderBoxDisplayType::None;
  else if (node->style.display == "block")
    display_type = RenderBoxDisplayType::Block;
  else if (node->style.display == "inline")
    display_type = RenderBoxDisplayType::Inline;

  // Compute border-width values (only top, right and left because bottom can't be used until height is computed)
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.;
  float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.;
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.;

  // Compute width and height
  if (display_type == RenderBoxDisplayType::Block) 
  {
    // If display = block, both the content width and the box width occupy occupy all of the possible space
    // We have to account for margin, padding and border width to compute content_width
    box_width = container_width;

    if (node->style.width == -1) 
    {
      // width is set to auto = computed automatically
      content_width = box_width - node->style.margin_right - border_right_value - node->style.padding_right
                                - node->style.margin_left - border_left_value - node->style.padding_left;
    } 
    else 
    {
      content_width = node->style.width;
      content_height = node->style.height;
    }
  }
  else if (display_type == RenderBoxDisplayType::Inline)
  {
    // If display = inline, box_width and content_width are computed based on the content-width, wich can't
    // be computed in yet (printable objects are not layed out). To compute values, we have to wait for the reflow()
  }

  // Determine (x,y) position
  auto [xref, yref] = compute_xy_reference();
  x = xref + node->style.margin_left + border_left_value + node->style.padding_left;
  y = yref + node->style.margin_top + border_top_value + node->style.padding_top;
  
  /*
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

  // margin-top collapsing from top-sibilings
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

  // margin-top collapsing from container
  float margin_top_accumulated = 0.;
  RenderBox* pr = parent;
  while (pr != nullptr and parent->children.size() == 0)
  {
    margin_top_accumulated += pr->node->style.margin_top;
    pr = pr->parent;
  }
  
  if (margin_top_accumulated > 0.)
  {
    float max_margin = std::max<float>(node->style.margin_top, margin_top_accumulated);
    float difference = (node->style.margin_top + margin_top_accumulated) - max_margin;

    y -= difference;
    margin.top -= difference;
    border.top -= difference;
    padding.top -= difference;
  }
  */
}

void RenderBox::reflow(float upstream_width)
{
  if (display_type == RenderBoxDisplayType::Inline)
  {
    float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.;
    float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.;

    content_width = upstream_width;
    box_width = content_width + node->style.margin_left + border_left_value + node->style.padding_left
                              + node->style.margin_right + border_right_value + node->style.padding_right;
  }
}

void RenderBox::compute_height(float accumulated_height)
{
  // Top and Bottom border-width value
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.;
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.;

  // Compute padding border and margin bottom edge
  //padding.bottom = y + accumulated_height + node->style.padding_bottom;
  //border.bottom = padding.bottom + border_bottom_value;
  //margin.bottom  = border.bottom + node->style.margin_bottom;


  content_height = accumulated_height;

  if (display_type == RenderBoxDisplayType::Inline)
  {
    // Apparently, in Inline elements, margin and padding top/bottom are not applied
    // thus not modifying the height of box
    box_height = accumulated_height + border_top_value + border_bottom_value;
  }
  else
  {
    box_height = accumulated_height + node->style.margin_top + border_top_value + node->style.padding_top +
                                      node->style.margin_bottom + border_bottom_value + node->style.padding_bottom;
  }

}

float RenderBox::get_ref_y() const
{
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.;
  return y - node->style.margin_top - border_top_value - node->style.padding_top;
}

float RenderBox::get_vertical_separation() const
{
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.;

  if (display_type == RenderBoxDisplayType::Inline)
    return content_height;
  else if (display_type == RenderBoxDisplayType::Block)
    return content_height + node->style.padding_bottom + border_bottom_value + node->style.margin_bottom;
}


void RenderBox::print(int number_tabs)
{
  for (int i = 0; i < number_tabs; ++i)
  {
    std::cout << "  ";
  }

  //std::cout << "(" << node->element_value << "): (" << x << "," << y << ")"  << std::endl;
  printf("(%s): (%.1f, %.1f) w=%.1f h=%.1f cw=%.1f ch=%.1f\n", node->element_value.c_str(), x, y, box_width, box_height, content_width, content_height);

  // std::cout << "(" << node->element_value << ") RenderBox: (" << x << "," << y << ") width=" << width << " height="  << height << 
    // " margin=(" << margin.top << " " << margin.right << " " << margin.bottom << " " << margin.left << ")" << 
    // " padding=(" << padding.top << " " << padding.right << " " << padding.bottom << " " << padding.left << ")" << std::endl;

  for (auto child : children)
  {
    child->print(number_tabs+1);
  }
}

}
