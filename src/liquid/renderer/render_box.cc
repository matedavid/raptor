#include "render_box.h"

namespace liquid 
{

/* ===== UTIL ===== */
float space_width(float font_size) 
{
  sf::Font font;
  if (not font.loadFromFile("../src/gui/Fonts/LiberationSans-Regular.ttf"))
    std::cout << "Error loading font" << std::endl;

  sf::Text t(" ", font, (uint)font_size);
  return t.getLocalBounds().width;

}

// Returns if a children is blocking the parent/first-child margin collapsing
bool has_child_blocking_mc(const std::vector<RenderBox*>& children)
{
  if (children.empty())
    return false;

  for (auto child : children)
    if (child->in_document_flow() and child->get_content_height() != 0.f) return true;

  return false;
}

// Returns the first children that is in the normal document flow from vector
RenderBox* first_children_in_flow(const std::vector<RenderBox*>& children)
{
  for (auto iter = children.rbegin(); iter != children.rend(); ++iter)
  {
    if ((*iter)->in_document_flow()) return *iter;
  }
  return nullptr;
}
/* ===== ==== ===== */


RenderBox::RenderBox()
{
}

RenderBox::RenderBox(HTMLElement* element, RenderBox* parent)
  : node(element), parent(parent)
{
}

std::pair<float, float> RenderBox::compute_xy_reference() const
{
  if (parent == nullptr)
    return { 0, 0 };

  std::vector<RenderBox*> siblings = parent->children;
  if (siblings.empty())
    return { parent->get_x(), parent->get_y() };

  RenderBox* last_sibling = siblings[siblings.size()-1];
  if (last_sibling->display_type == RenderBoxDisplayType::Inline and display_type == RenderBoxDisplayType::Inline)
  {
    float xref = last_sibling->get_x() + last_sibling->get_box_width() + space_width(node->style.font_size);
    float yref = last_sibling->get_ref_y();

    return { xref, yref };
  }
  else if (not last_sibling->in_flow)
  {
    // If last_sibling is not in the normal document flow, get the next direct sibling that it's in the document flow 
    // if exists, otherwise, use the parent as reference
    int idx = siblings.size()-1;
    while (idx >= 0)
    {
      RenderBox* sibling = siblings[idx--];
      if (not sibling->in_flow)
        continue;

      //float xref = sibling->get_x() + sibling->get_box_width();
      float xref = sibling->xref;
      float yref = sibling->yref + sibling->get_box_height();

      return { xref, yref };
    }
    
    if (parent == nullptr)
      return { 0, 0 };

    // Using parent as reference
    return { parent->get_x(), parent->get_y() };
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

  // Specify and set position
  if (node->style.position == "static")
    position = RenderBoxPosition::Static;
  else if (node->style.position == "relative")
    position = RenderBoxPosition::Relative;
  else if (node->style.position == "absolute")
    position = RenderBoxPosition::Absolute;
  else if (node->style.position == "fixed")
    position = RenderBoxPosition::Fixed;
  else if (node->style.position == "sticky")
    position = RenderBoxPosition::Sticky;

  // Set if render_box is in the normal document flow
  in_flow = position != RenderBoxPosition::Absolute and position != RenderBoxPosition::Fixed;

  // Compute border-width values (only top, right and left because bottom can't be used until height is computed)
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.f;
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;

  // Compute width and height
  if (display_type == RenderBoxDisplayType::Block) 
  {
    // If display = block, the width occupies all the possible space
    // We have to account for margin, padding and border width to compute content_width
    if (node->style.width == -1) 
    {
      // width is set to auto => computed automatically
      content_width = container_width - node->style.margin_right - border_right_value - node->style.padding_right
                                      - node->style.margin_left - border_left_value - node->style.padding_left;
    } 
    else
    {
      content_width = node->style.width;
    }

    // To compute height we need to wait for compute_height() call, except if it's specified
    if (node->style.height != -1) 
    {
      content_height = node->style.height;
    }
  }
  else if (display_type == RenderBoxDisplayType::Inline)
  {
    // If display = inline, the width is computed  based on the children's width, which can't
    // be computed yet (printable objects have not been layed out). To compute values, we have to wait for the reflow()
  }

  // Determine (x,y) position
  auto [_xref, _yref] = compute_xy_reference();
  xref = _xref;
  yref = _yref;

  if (position == RenderBoxPosition::Absolute)
  {
    x = xref + node->style.margin_left + border_left_value + node->style.padding_left;
    y = yref + node->style.margin_top + border_top_value + node->style.padding_top;

    y += node->style.top;
    y -= node->style.bottom;
    x += node->style.left;
    x -= node->style.right;
    return;
  }

  float margin_top_apply = node->style.margin_top;
  // Margin collapsing for adjacent siblings
  if (display_type != RenderBoxDisplayType::Inline)
  {
    float adj_margin_bottom = 0.f;

    // Get top element in the render tree
    RenderBox* top_render_box = this;
    while (top_render_box->parent != nullptr)
      top_render_box = top_render_box->parent;

    // Get the direct sibling reference value
    RenderBox* direct_sibling = this;
    // TODO: look for better solution to check if first_children_in_flow is nullptr instead of calling the same function twice
    if (parent != nullptr and not parent->children.empty() and first_children_in_flow(parent->children) != nullptr)
    {
      // Adjacent sibling under same container
      direct_sibling = first_children_in_flow(parent->children);
      adj_margin_bottom = direct_sibling->node->style.margin_bottom;

      /*
      direct_sibling = parent->children[parent->children.size() - 1];
      adj_margin_bottom = direct_sibling->node->style.margin_bottom;S
      */
    }
    else if (parent != top_render_box and not top_render_box->children.empty() and first_children_in_flow(top_render_box->children) != nullptr)
    {
      // Adjacent sibling in all the tree (different containers)
      direct_sibling = first_children_in_flow(top_render_box->children);
      adj_margin_bottom = direct_sibling->node->style.margin_bottom;

      /*
      direct_sibling = top_render_box->children[top_render_box->children.size()-1]; // Get the first child of current node's parent
      adj_margin_bottom = direct_sibling->node->style.margin_bottom;
      */
    }

    // Compute the adj_margin_bottom_value from the direct sibling (maximum of all the margins of all the margins)
    while (not direct_sibling->children.empty() and direct_sibling->children[direct_sibling->children.size()-1]->type() == RenderBoxType::Default)
    {
      adj_margin_bottom = std::max<float>(adj_margin_bottom, direct_sibling->node->style.margin_bottom);
      direct_sibling = direct_sibling->children[direct_sibling->children.size()-1];
    }

    float distance = yref - (direct_sibling->get_y()+direct_sibling->get_height());

    float max = std::max<float>(adj_margin_bottom, node->style.margin_top);
    float translate = max-distance;

    if (this != direct_sibling and direct_sibling->in_flow)
    {
      //std::cout << "(" << node->element_value << "," << node->id << ") Adjacent sibling: " << translate << std::endl;
      adjacent_sibling_mc = true;
      margin_top_apply = translate;
    }
  }

  // Margin collapsing for parent and first/last child elements
  if (display_type != RenderBoxDisplayType::Inline and node->style.margin_top != 0.f 
      and parent != nullptr and not has_child_blocking_mc(parent->children) 
      and parent->node->style.padding_top == 0.f and parent->node->style.border_style[0] == "none")
  {
    float parent_mt = parent->node->style.margin_top;

    RenderBox* p = parent;
    while (p != nullptr)
    {
      if (not p->children.empty() and first_children_in_flow(p->children) != nullptr)
        break;

      parent_mt = std::max<float>(parent_mt, p->node->style.margin_top);
      p = p->parent;
    }

    margin_top_apply = margin_top_apply-parent_mt; // TODO: Not sure this is ok

    // Apply margin collapsing to parents
    p = parent;
    while (p != nullptr and p->children.empty())
    {
      parent_first_child_mc = true;

      p->y += margin_top_apply;
      p = p->parent;
    }
  }

  if (display_type == RenderBoxDisplayType::Inline)
  {
    x = xref + border_left_value;
    y = yref + border_top_value;
  }
  else
  {
    x = xref + node->style.margin_left + border_left_value + node->style.padding_left;
    y = yref + margin_top_apply + border_top_value + node->style.padding_top;
  }

  // position = relative
  if (position == RenderBoxPosition::Relative)
  {
    y += node->style.top;
    y -= node->style.bottom;
    x += node->style.left;
    x -= node->style.right;
  }
}

void RenderBox::reflow(float upstream_width)
{
  if (display_type == RenderBoxDisplayType::Inline)
  {
    float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.f;
    float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;

    content_width = upstream_width;
    /*
    box_width = content_width + node->style.margin_left + border_left_value + node->style.padding_left
                              + node->style.margin_right + border_right_value + node->style.padding_right;
    */
  }
}

void RenderBox::compute_height(float accumulated_height)
{

  if (content_height != -1)
    return;

  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.f;

  content_height = accumulated_height;
}

float RenderBox::get_border_x() const
{
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;
  return x - node->style.padding_left - border_left_value;
}

float RenderBox::get_border_y() const
{
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  return y - node->style.padding_top - border_top_value;
}

float RenderBox::get_ref_y() const
{
  //return compute_xy_reference().second;
  return yref;
}

float RenderBox::get_box_width() const
{ 
  float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.f;
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;

  return content_width + node->style.margin_left + border_left_value + node->style.padding_left
                       + node->style.margin_right + border_right_value + node->style.padding_right;
}

float RenderBox::get_box_height() const
{
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.f;

  // Apparently, in Inline elements, margin and padding top/bottom are not applied
  // thus not modifying the height of box
  if (display_type == RenderBoxDisplayType::Inline)
    return content_height + border_top_value + border_bottom_value;
  else if (adjacent_sibling_mc)
    return content_height + border_top_value + node->style.padding_top
                          + node->style.margin_bottom + border_bottom_value + node->style.padding_bottom;

  return content_height + node->style.margin_top + border_top_value + node->style.padding_top
                        + node->style.margin_bottom + border_bottom_value + node->style.padding_bottom;
}

float RenderBox::get_width() const
{
  float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.f;
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;

  return content_width + node->style.padding_left + node->style.padding_right + border_left_value + border_right_value;
}

float RenderBox::get_height() const
{
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.f;

  return content_height + node->style.padding_top + node->style.padding_bottom + border_top_value + border_bottom_value;
}

std::vector<std::pair<float, float>> RenderBox::get_border_edges() const
{
  float border_top_value = node->style.border_style[0] != "none" ? resolve_border_width(node->style.border_width[0]) : 0.f;
  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.f;
  float border_right_value = node->style.border_style[1] != "none" ? resolve_border_width(node->style.border_width[1]) : 0.f;
  float border_left_value = node->style.border_style[3] != "none" ? resolve_border_width(node->style.border_width[3]) : 0.f;

  float x_left = x - node->style.padding_left - border_left_value;
  float x_right = x + content_width + node->style.padding_right;

  float y_top = y - node->style.padding_top - border_top_value;
  float y_bottom = y + content_height + node->style.padding_bottom + border_bottom_value;

  auto top_left = std::make_pair(x_left, y_top);
  auto top_right = std::make_pair(x_right, y_top);
  auto bottom_left = std::make_pair(x_left, y_bottom);
  auto bottom_right = std::make_pair(x_right, y_bottom);

  return { top_left, top_right, bottom_left, bottom_right };
}

float RenderBox::get_vertical_separation() const
{
  if (display_type == RenderBoxDisplayType::Inline)
    return content_height;

  float border_bottom_value = node->style.border_style[2] != "none" ? resolve_border_width(node->style.border_width[2]) : 0.f;
  return content_height + node->style.padding_bottom + border_bottom_value + node->style.margin_bottom;
}


void RenderBox::print(int number_tabs)
{
  for (int i = 0; i < number_tabs; ++i)
  {
    std::cout << "  ";
  }

  printf("(%s): (%.1f, %.1f) bw=%.1f bh=%.1f w=%.1f h=%.1f\n", node->element_value.c_str(), x, y, get_box_width(), get_box_height(), get_width(), get_height());

  for (auto child : children)
  {
    child->print(number_tabs+1);
  }
}

}
