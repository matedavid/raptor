#include "render_tree.h"

namespace liquid {

float generate_text_tree(Text* text, RenderBox* parent, float width)
{
  RenderBoxText* render_box_text = new RenderBoxText(text, parent);
  render_box_text->layout(width);

  float text_height = 0.;
  while (render_box_text->get_box_width() >= width)
  {
    std::string split_content = render_box_text->split_content(width);

    parent->add_child(render_box_text);
    text_height += render_box_text->get_box_height();

    render_box_text = new RenderBoxText(text, parent);
    render_box_text->layout(width);
    render_box_text->set_content(split_content);
  }

  parent->add_child(render_box_text);
  text_height += render_box_text->get_box_height();

  return text_height;
}

RenderBox* generate_render_tree(HTMLElement* element, RenderBox* parent, float width)
{
  // Generate Image
  if (element->element_value == "img")
  {
    HTMLImageElement* img_element = dynamic_cast<HTMLImageElement*>(element);
    if (img_element == nullptr)
      return nullptr;

    RenderBoxImage* render_box_image = new RenderBoxImage(img_element, parent);
    render_box_image->layout(width);
    return render_box_image;
  }

  RenderBox* render_box = new RenderBox(element, parent);
  render_box->layout(width);

  float accumulated_height = 0.f;
  for (HTMLElement* child : element->child_elements())
  {
    float container_width = render_box->get_content_width();
    if (render_box->get_display_type() == RenderBoxDisplayType::Inline)
      container_width = width;

    // Special treatment for text
    if (child->type() == HTMLElementType::TextType)
    {
      Text* text = dynamic_cast<Text*>(child);
      if (text == nullptr)
        continue;

      accumulated_height += generate_text_tree(text, render_box, container_width);
      continue;
    }

    RenderBox* render_box_child = generate_render_tree(child, render_box, container_width);
    render_box->add_child(render_box_child);

    std::vector<RenderBox*> children = render_box->get_children();
    // Case where we have to compute height of two inline elements
    if (children.size() > 1 and render_box_child->get_display_type() == RenderBoxDisplayType::Inline 
        and children[children.size()-2]->get_display_type() == RenderBoxDisplayType::Inline)
    {
      RenderBox* last_child = children[children.size()-2];

      std::cout << "two inline elements" << std::endl;

      // Removing last_child height to update it with the following max comparison
      accumulated_height -= last_child->get_box_height();

      if (not render_box_child->in_document_flow() and not last_child->in_document_flow())
        continue;

      float max = std::max<float>(render_box_child->get_box_height(), last_child->get_box_height());
      if (not render_box_child->in_document_flow())
        max = last_child->get_box_height();
      else if (not last_child->in_document_flow())
        max = render_box_child->get_box_height();

      accumulated_height += max;
    }
    else if (render_box_child->get_display_type() != RenderBoxDisplayType::Inline)
    {
      if (not render_box_child->in_document_flow())
        continue;

      // Margin collapsing with parent and first-child
      //if (render_box->get_y() == render_box_child->get_ref_y() and render_box_child->node->style.margin_top != 0.f)
      if (render_box_child->parent_first_child_mc)
        accumulated_height += render_box_child->get_height();
      else
        accumulated_height += render_box_child->get_box_height();
    }
  }

  render_box->compute_height(accumulated_height);

  // Apply reflow() if render_box has display_type = inline (width computing in inline elements)
  if (render_box->get_display_type() == RenderBoxDisplayType::Inline)
  {
    float upstream_width = 0;
    // TODO: This looks way too simple, probably doesn't take into account all possibles scenarios
    for (RenderBox* child : render_box->get_children())
    {
      if (child->get_display_type() == RenderBoxDisplayType::Block)
      {
        upstream_width = child->get_box_width();
        break;
      }
      else if (child->get_display_type() == RenderBoxDisplayType::Inline or child->type() == RenderBoxType::Txt)
        upstream_width += child->get_box_width();
    }

    render_box->reflow(upstream_width);
  }

  return render_box;
}

}
