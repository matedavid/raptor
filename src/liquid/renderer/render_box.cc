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

  printf("[RenderBox (%s)]\n", node->element_value.c_str());
  
  for (auto child : children) 
    child->print(n_tabs+1);
}

}
