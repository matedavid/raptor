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

  float padding_top = node->style.padding_top;
  float padding_bottom = node->style.padding_bottom;
  float padding_left = node->style.padding_left;
  float padding_right = node->style.padding_right;

  float border_width_top = node->style.border_style.top != "none" ? node->style.border_width.top : 0.f;
  float border_width_bottom = node->style.border_style.bottom != "none" ? node->style.border_width.bottom : 0.f;
  float border_width_left = node->style.border_style.left != "none" ? node->style.border_width.left : 0.f;
  float border_width_right = node->style.border_style.right != "none" ? node->style.border_width.right : 0.f;

  float margin_top = node->style.margin_top;
  float margin_bottom = node->style.margin_bottom;
  float margin_left = node->style.margin_left;
  float margin_right = node->style.margin_right;

  // NOTE: Every alternative should give value to:
  // - content_height, height, margin_height 
  // - content_width, width, margin_width
  if (display == RenderBoxDisplay::Block)
  {
    content_height = height_val == -1.f ? accumulated_height : height_val;
    height = content_height + padding_top + padding_bottom + border_width_top + border_width_bottom;
    margin_height = height + margin_top + margin_bottom;

    margin_width = container_width;
    width = margin_width - margin_left - margin_right;
    content_width = width_val == -1.f ? width - padding_left - padding_right - border_width_left - border_width_right
                                      : width_val;
  }
  else if (display == RenderBoxDisplay::Inline)
  {
    // NOTE: Even tho vertical height and padding are applied (border_width works different), 
    // they do not push away other elements
    content_height = accumulated_height;
    height = content_height + padding_top + padding_bottom + border_width_top + border_width_bottom;
    margin_height = height + margin_top + margin_bottom;

    content_width = accumulated_width;
    width = content_width + padding_left + padding_right + border_width_left + border_width_right;
    margin_width = width + margin_left + margin_right;
  }

  return Dimensions{.width=width, .height=height};
}

void RenderBox::layout()
{
}

void RenderBox::insert_child(RenderBox* child)
{
  children.push_back(child);
}

void RenderBox::print(int n_tabs)
{
  for (int i = 0; i < n_tabs; ++i)
    std::cout << "  ";

  printf("[RenderBox (%s) w=(%.1f,%.1f,%.1f) h=(%.1f,%.1f,%.1f)]\n", node->element_value.c_str(), 
          margin_width, width, content_width, 
          margin_height, height, content_height
        );
  
  for (auto child : children) 
    child->print(n_tabs+1);
}

}
