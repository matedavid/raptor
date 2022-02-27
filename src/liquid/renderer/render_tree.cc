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

  RenderBox* render_box = new RenderBox(element, parent);
  render_box->layout(width);

  float accumulated_height = 0.f;
  for (HTMLElement* child : element->child_elements())
  {
    RenderBox* render_box_child = generate_render_tree(child, render_box, render_box->get_width());
    if (render_box_child->get_type() != RenderBoxType::Inline)
      accumulated_height += render_box_child->get_height();

    render_box->add_child(render_box_child);
  }

  float height = accumulated_height + element->style.margin_top + element->style.padding_top + 
                                      element->style.margin_bottom + element->style.padding_bottom;
  render_box->set_height(height);

  return render_box;
}

}
