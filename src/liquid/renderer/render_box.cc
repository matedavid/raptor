#include "render_box.h"

namespace liquid {

RenderBox::RenderBox(HTMLElement* node, RenderBox* parent)
  : node(node), parent(parent)
{
  std::string n_display = node->style.display;
  if (n_display == "none")
    display = RenderBoxDisplay::None;
  else if (n_display == "block")
    display = RenderBoxDisplay::Block;
  else if (n_display == "inline")
    display = RenderBoxDisplay::Inline;
  else if (n_display == "list-item")
    display = RenderBoxDisplay::ListItem;
  
  std::string n_position = node->style.position;
  if (n_position == "static")
    position = RenderBoxPosition::Static;
  else if (n_position == "relative")
    position = RenderBoxPosition::Relative;
  else if (n_position == "absolute")
    position = RenderBoxPosition::Absolute;
  else if (n_position == "fixed")
    position = RenderBoxPosition::Fixed;
  else if (n_position == "sticky")
    position = RenderBoxPosition::Sticky;

  padding.top = node->style.padding_top;
  padding.bottom = node->style.padding_bottom;
  padding.left = node->style.padding_left;
  padding.right = node->style.padding_right;

  border_width.top = node->style.border_style.top != "none" ? node->style.border_width.top : 0.f;
  border_width.bottom = node->style.border_style.bottom != "none" ? node->style.border_width.bottom : 0.f;
  border_width.left = node->style.border_style.left != "none" ? node->style.border_width.left : 0.f;
  border_width.right = node->style.border_style.right != "none" ? node->style.border_width.right : 0.f;

  margin.top = node->style.margin_top;
  margin.bottom = node->style.margin_bottom;
  margin.left = node->style.margin_left;
  margin.right = node->style.margin_right;
}

Dimensions RenderBox::compute_dimensions(float container_width)
{
  float accumulated_height = 0.f;
  float accumulated_width  = 0.f;
  RenderBoxDisplay prev_display = RenderBoxDisplay::None;
  for (RenderBox* child : children)
  {
    Dimensions dims = child->compute_dimensions(container_width);

    // height
    accumulated_height += dims.height;

    // width 
    if (child->display == RenderBoxDisplay::Inline and prev_display == RenderBoxDisplay::Inline)
      accumulated_width += dims.width;
    else
      accumulated_width = dims.width;

    prev_display = child->display;
  }

  float height_val = node->style.height;
  float width_val  = node->style.width;


  // NOTE: Every alternative should give value to:
  // - content_height, height, margin_height 
  // - content_width, width, margin_width
  if (display == RenderBoxDisplay::Block)
  {
    content_height = height_val == -1.f ? accumulated_height : height_val;
    height = content_height + padding.top + padding.bottom + border_width.top + border_width.bottom;
    margin_height = height + margin.top + margin.bottom;

    margin_width = container_width;
    width = margin_width - margin.left - margin.right;
    content_width = width_val == -1.f ? width - padding.left - padding.right - border_width.left - border_width.right
                                      : width_val;
  }
  else if (display == RenderBoxDisplay::Inline)
  {
    // NOTE: Even tho vertical height and padding are applied (border_width works different), 
    // they do not push away other elements
    content_height = accumulated_height;
    height = content_height + padding.top + padding.bottom + border_width.top + border_width.bottom;
    margin_height = height + margin.top + margin.bottom;

    content_width = accumulated_width;
    width = content_width + padding.left + padding.right + border_width.left + border_width.right;
    margin_width = width + margin.left + margin.right;
  }

  return Dimensions{.width=width, .height=height};
}

void RenderBox::layout()
{
  x, y = 0.f;
}

void RenderBox::insert_child(RenderBox* child)
{
  children.push_back(child);
}

void RenderBox::print(int n_tabs)
{
  for (int i = 0; i < n_tabs; ++i)
    std::cout << "  ";

  printf("[RenderBox (%s) pos=(%.1f, %.1f) w=(%.1f,%.1f,%.1f) h=(%.1f,%.1f,%.1f)]\n", node->element_value.c_str(),
          x, y,
          margin_width, width, content_width, 
          margin_height, height, content_height
        );
  
  for (auto child : children) 
    child->print(n_tabs+1);
}

}
