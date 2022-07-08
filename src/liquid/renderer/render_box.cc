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

float RenderBox::compute_height()
{
  float accumulated_height = 0.f;
  for (RenderBox* child : children)
    accumulated_height += child->compute_height();

  float height_val = node->style.height;

  float padding_top = node->style.padding_top;
  float padding_bottom = node->style.padding_bottom;

  float border_width_top = node->style.border_style.top != "none" ? node->style.border_width.top : 0.f;
  float border_width_bottom = node->style.border_style.bottom != "none" ? node->style.border_width.bottom : 0.f;

  float margin_top = node->style.margin_top;
  float margin_bottom = node->style.margin_bottom;

  // Note: Every alternative should give value to:
  // - content_height
  // - height
  // - margin_height 
  if (display == RenderBoxDisplay::Block)
  {
    content_height = height_val == -1. ? accumulated_height : height_val;
    height = content_height + padding_top + padding_bottom + border_width_top + border_width_bottom;
    margin_height = height + margin_top + margin_bottom;
  }
  else if (display == RenderBoxDisplay::Inline)
  {
    // TODO:
  }

  return height;
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

  printf("[RenderBox (%s) h=(%.1f,%.1f,%.1f)]\n", node->element_value.c_str(), margin_height, height, content_height);
  
  for (auto child : children) 
    child->print(n_tabs+1);
}

}
