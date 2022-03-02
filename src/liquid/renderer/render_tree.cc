#include "render_tree.h"

namespace liquid {

RenderBox* generate_render_tree(HTMLElement* element, RenderBox* parent, float width)
{
  if (element->type() == HTMLElementType::TextType)
  {
    Text* text_element = dynamic_cast<Text*>(element);
    if (text_element == nullptr)
      return nullptr;

    RenderBoxText* render_box_text = new RenderBoxText(text_element, parent);
    return render_box_text;
  }
  else if (element->element_value == "img")
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
    RenderBox* render_box_child = generate_render_tree(child, render_box, render_box->get_width());
    if (render_box_child->get_display_type() != RenderBoxDisplayType::Inline)
      accumulated_height += render_box_child->get_height();

    render_box->add_child(render_box_child);
  }

  render_box->compute_height(accumulated_height);

  return render_box;
}

}
