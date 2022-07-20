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
  else if (n_display == "inline-block")
    display = RenderBoxDisplay::InlineBlock;
  
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

  in_document_flow = (position == RenderBoxPosition::Static or 
                      position == RenderBoxPosition::Relative or 
                      position == RenderBoxPosition::Sticky);

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

  x = y = 0.f;
}

AppliedDimensions RenderBox::compute_dimensions(float container_width)
{
  AppliedDimensions applied;

  float height_val = node->style.height;
  float width_val  = node->style.width;

  if (display == RenderBoxDisplay::Block || display == RenderBoxDisplay::InlineBlock)
  {
    if (height_val != AUTO)
    {
      content_height = height_val;
      height = content_height + padding.top + padding.bottom + border_width.top + border_width.bottom;
    }

    // If width = auto, width and content width are constrained by the container_width.
    // If width is set to a value the reverse happens, width and content_width expand based on the specified value.
    if (width_val == AUTO)
    {
      width = container_width - margin.left - margin.right;
      content_width = width - padding.left - padding.right - border_width.left - border_width.right;
    } 
    else
    {
      content_width = width_val;
      width = content_width + padding.left + padding.right + border_width.left + border_width.right;
    }

    applied.height = (height_val != AUTO);
    applied.width  = true;
  }
  else if (display == RenderBoxDisplay::Inline)
  {
    applied.height = false;
    applied.width  = false;
  }

  return applied;
}

void RenderBox::layout_lines(std::vector<Line>& lines, std::vector<RenderBox*>& not_in_flow) const
{
  for (RenderBox* child : children)
  {
    if (not child->in_document_flow)
    {
      not_in_flow.push_back(child);
    }
    else if (child->display == RenderBoxDisplay::Block)
    {
      Line new_line = Line{.horizontal=false, .elements={child}};
      lines.push_back(new_line);
    }
    else if (child->display == RenderBoxDisplay::Inline or child->display == RenderBoxDisplay::InlineBlock)
    {
      if (not lines.empty() && lines[lines.size()-1].horizontal)
      {
        lines[lines.size()-1].elements.push_back(child);
      }
      else
      {
        Line new_line = Line{.horizontal=true, .elements={child}};
        lines.push_back(new_line);
      }
    }
  }
}

RenderBox::LayoutResult RenderBox::layout(LayoutParameters params)
{
  // Initialize x and y values
  x = params.xref;
  y = params.yref;

  AppliedDimensions applied_dims = this->compute_dimensions(params.container_width, params.container_height);

  // Get the line representation of the render_box's children
  std::vector<Line> lines;
  std::vector<RenderBox*> not_in_flow;
  layout_lines(lines, not_in_flow);

  // Apply horizontal margin offset
  x += margin.left;

  // Position
  if (position == RenderBoxPosition::Relative)
  {
  	// left has priority over right
    float horizontal_offset = 0.f;
    if (node->style.left != AUTO)
      horizontal_offset = node->style.left;
    else if (node->style.right != AUTO)  
      horizontal_offset = -node->style.right;  

  	// top has priority over bottom
    float vertical_offset = 0.f;
    if (node->style.top != AUTO)
      vertical_offset = node->style.top;
    else if (node->style.bottom != AUTO)  
      vertical_offset = -node->style.bottom;  

    x += horizontal_offset;
    y += vertical_offset;
  }
  else if (position == RenderBoxPosition::Absolute)
  {
    if (parent == nullptr)
    {
      x = 0.f;
      y = 0.f;
    }
    else if (not parent->children.empty() and parent->children[0] != this)
    {
      // Find the direct sibling of the current element
      RenderBox* sibling = parent->children[0];
      int idx = 0;
      while (idx+1 < parent->children.size() and parent->children[idx+1] != this)
      {
        sibling = parent->children[idx+1];
        ++idx;
      }
      if (display == RenderBoxDisplay::Inline and sibling->display == RenderBoxDisplay::Inline)
      {
        x = sibling->x + sibling->width;
        y = sibling->y;
      }
      else 
      {
        float sibling_margin_bottom = sibling->display != RenderBoxDisplay::Inline ? sibling->margin.bottom : 0.f;
        x = sibling->x;
        y = sibling->y + sibling->height + sibling_margin_bottom;
      }
    }
    else
    {
      x = parent->x;
      y = parent->y;
    }

    // TODO: implement top, bottom, left, right css attributes
    if (node->style.top != AUTO) // auto
    {
      //y = top;
    }
    else if (node->style.bottom != AUTO)
    {
      //y = params.container_height - bottom;
    }

    x += margin.left + border_width.left + padding.left;
    y += margin.top + border_width.top + padding.top;
  }
  else if (position == RenderBoxPosition::Fixed)
  {
    // TODO: Implement
  }
  else if (position == RenderBoxPosition::Sticky)
  {
    // TODO: Implement
  }

  // Define result struct
  LayoutResult result;

  // First child margin collapsing
  if (parent == nullptr and display == RenderBoxDisplay::Block)
  {
    y += margin.top;
    params.margin_top_applied = margin.top;
  }
  else if (display == RenderBoxDisplay::Block and parent->is_first_child(this) and margin.top > params.margin_top_applied)
  {
    float remaining = margin.top - params.margin_top_applied;
    y += remaining;

    params.margin_top_applied = margin.top;
    result.margin_top_remaining = remaining;
  }
  else if (display == RenderBoxDisplay::InlineBlock)
  {
    y += margin.top;
  }

  // Compute position
  float height_offset = 0.f;
  for (int idx = 0; idx < lines.size(); ++idx)
  {
    Line line = lines[idx];
    float line_height = 0.f;

    LayoutResult child_result;
    if (not line.horizontal)
    {
      RenderBox* child = line.elements[0];
      params.xref = x;
      params.yref = y + height_offset;
      params.container_width = content_width;

      child_result = child->layout(params);
      // Propagate margin collapsing upstream
      if (child_result.margin_top_remaining != 0.f and idx == 0)
      {
        y += child_result.margin_top_remaining;
        result.margin_top_remaining = child_result.margin_top_remaining;
      }
      // Propagate the resulting margin top/bottom upwards 
      result.resulting_margin_top = child_result.resulting_margin_top;
      result.resulting_margin_bottom = child_result.resulting_margin_bottom;

      line_height = child->height;
    }
    else
    {
      float width_offset = 0.f;
      for (int child_id = 0; child_id < line.elements.size(); ++child_id)
      {
        RenderBox* child = line.elements[child_id];

        params.xref = x + width_offset;
        params.yref = y + height_offset;
        child_result = child->layout(params);

        line_height = std::max<float>(child->height, line_height);

        // TODO: This is a temporal solution
        // As InlineBlock elements apply their own margin-top, if they do apply it they'll position
        // themselves correctly, but the parent element (the current element) won't know that it has 
        // pushed himself down, therefore the height_offset wont have the correct value to position
        // the next element. Adding the margin-top to the line_height to correct the height_offset
        if (child->display == RenderBoxDisplay::InlineBlock)
        {
          line_height += child->margin.top;
        }

        width_offset += child->width + child->margin.left + child->margin.right;

        // If element is not the last element in the line, add to the offset
        // the needed separation between elements
        if (child_id != line.elements.size()-1)
        {
          // Default value is based on the width of a space character (" ")
          // with 16px font size
          float SPACE_WIDTH = 4.f;
          width_offset += SPACE_WIDTH;
        }
      }

      // Give value to width if couldn't be done in compute_dimensions
      if (not applied_dims.width)
      {
        content_width = width_offset;
        width = content_width + padding.left + padding.right + border_width.left + border_width.right;
      }
    }

    // Adjacent siblings margin collapsing
    float adjacent_siblings_margin = 0.f;
    if (idx+1 < lines.size())
    {
      Line next_line = lines[idx+1];

      RenderBoxDisplay display_current_line = line.elements[0]->display;
      RenderBoxDisplay display_next_line = next_line.elements[0]->display;
      
      float current_line_margin_bottom = child_result.resulting_margin_bottom;

      float next_line_margin_top = next_line.elements[0]->margin.top;
      if (display_next_line == RenderBoxDisplay::Block)
      {
        LayoutResult tmp_res = next_line.elements[0]->layout({});
        next_line_margin_top = std::max<float>(tmp_res.resulting_margin_top, next_line_margin_top);
      }

      if (display_current_line == RenderBoxDisplay::Block and display_next_line == RenderBoxDisplay::Block)
        adjacent_siblings_margin = std::max<float>(current_line_margin_bottom, next_line_margin_top);
      else if (display_current_line == RenderBoxDisplay::Block and display_next_line == RenderBoxDisplay::Inline)
        adjacent_siblings_margin = current_line_margin_bottom;
      else if (display_current_line == RenderBoxDisplay::Inline and display_next_line == RenderBoxDisplay::Block)
        adjacent_siblings_margin = next_line_margin_top;
      else if (display_current_line == RenderBoxDisplay::InlineBlock and display_next_line == RenderBoxDisplay::Block)
        adjacent_siblings_margin = current_line_margin_bottom + next_line_margin_top;
      else if (display_current_line == RenderBoxDisplay::Block and display_next_line == RenderBoxDisplay::InlineBlock)
        adjacent_siblings_margin = current_line_margin_bottom; // InlineBlock elements apply their own margin-top
    }

    height_offset += line_height + adjacent_siblings_margin;

    // To prevent the following lines from applying margin top again, 
    // as it has already been applied with the adjacent sibling
    params.margin_top_applied = adjacent_siblings_margin;
  }

  // Give value to height if couldn't be done in compute_dimensions
  if (not applied_dims.height)
  {
    content_height = height_offset;
    height = content_height + padding.top + padding.bottom + border_width.top + border_width.bottom;
  }

  // Layout elements not in the normal document flow
  for (RenderBox* child : not_in_flow)
  {
    child->layout({});
  }

  result.resulting_margin_top = std::max<float>(margin.top, result.resulting_margin_top);
  result.resulting_margin_bottom = std::max<float>(margin.bottom, result.resulting_margin_bottom);

  return result;
}

void RenderBox::insert_child(RenderBox* child)
{
  children.push_back(child);
}

void RenderBox::print(int n_tabs)
{
  for (int i = 0; i < n_tabs; ++i)
    std::cout << "  ";

  printf("[RenderBox (%s) pos=(%.1f, %.1f) w=(%.1f,%.1f) h=(%.1f,%.1f)]\n", node->element_value.c_str(),
          x, y,
          width, content_width, 
          height, content_height
        );
  
  for (auto child : children) 
    child->print(n_tabs+1);
}

}
