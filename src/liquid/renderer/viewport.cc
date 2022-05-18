#include "viewport.h"

namespace liquid {

void Viewport::compute_lowest_y(RenderBox* tree, float& min)
{
  if (tree == nullptr)
    return;

  float computed_y = tree->get_y() + tree->get_content_height();
  min = std::max<float>(min, computed_y);

  for (auto child : tree->get_children())
    compute_lowest_y(child, min);
}

Viewport::Viewport(HTMLElement* element, float width, float height)
  : width(width), height(height), x(0.f), y(0.f), element_tree(element)
{
  render();
}

void Viewport::render()
{
  std::cout << "Hello there" << std::endl;
  tree = generate_render_tree(element_tree, nullptr, width);
  std::cout << "Hello there 2" << std::endl;
  lowest_y = 0.f;
  compute_lowest_y(tree, lowest_y);

  tree->print(0);
}

void Viewport::update_width(float new_width)
{
  width = new_width;
  render();
}

void Viewport::update_height(float new_height)
{
  height = new_height;
}

void Viewport::scroll_up()
{
  if (y <= 0) return;
  y -= scroll_factor;
}

void Viewport::scroll_down()
{
  if (y+height >= lowest_y) return;
  y += scroll_factor;
}

}