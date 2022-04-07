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
  if (siblings.empty())
    return { parent->get_x(), parent->get_y() };

  RenderBox* last_sibling = siblings[siblings.size()-1];
  if (last_sibling->display_type == RenderBoxDisplayType::Inline and display_type == RenderBoxDisplayType::Inline)
  {
    float xref = last_sibling->get_x() + last_sibling->get_width();
    float yref = last_sibling->get_ref_y();

    return { xref, yref };
  }

  // If last_sibling or current render_box has display_type = block, block starts in new line,
  // so yref has to take into account height of last_sibling
  float xref = last_sibling->get_x();
  float yref = last_sibling->get_y() + last_sibling->get_vertical_separation();

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
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.f;
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;

  // Compute width and height
  if (display_type == RenderBoxDisplayType::Block) 
  {
    // If display = block, both the content width and the box width occupy all the possible space
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
    // If display = inline, box_width and content_width are computed based on the content-width, which can't
    // be computed in yet (printable objects are not laid out). To compute values, we have to wait for the reflow()
  }

  // Determine (x,y) position
  auto [xref, yref] = compute_xy_reference();

  float margin_top_apply = node->style.margin_top;

  // Margin collapsing for adjacent siblings
  if (display_type != RenderBoxDisplayType::Inline)
  {
    //float adj_margin_top = 0.f;
    float adj_margin_bottom = 0.f;

    // Adjacent sibling under same container
    if (parent != nullptr and not parent->children.empty())
    {
      RenderBox* direct_sibling = parent->children[parent->children.size()-1];
      adj_margin_bottom = direct_sibling->node->style.margin_bottom;

      float max = std::max<float>(adj_margin_bottom, node->style.margin_top);
      float difference = max - adj_margin_bottom;
      std::cout << difference << std::endl;

      margin_top_apply = difference;
    }
  }

  x = xref + node->style.margin_left + border_left_value + node->style.padding_left;
  y = yref + margin_top_apply + border_top_value + node->style.padding_top;

}

void RenderBox::reflow(float upstream_width)
{
  if (display_type == RenderBoxDisplayType::Inline)
  {
    float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.f;
    float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;

    content_width = upstream_width;
    box_width = content_width + node->style.margin_left + border_left_value + node->style.padding_left
                              + node->style.margin_right + border_right_value + node->style.padding_right;
  }
}

void RenderBox::compute_height(float accumulated_height)
{
  // Top and Bottom border-width value
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.f;

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
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  return y - node->style.margin_top - border_top_value - node->style.padding_top;
}

float RenderBox::get_vertical_separation() const
{
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.f;

  if (display_type == RenderBoxDisplayType::Inline)
    return content_height;

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
