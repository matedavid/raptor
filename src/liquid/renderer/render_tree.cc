#include "render_tree.h"

namespace liquid {

RenderBox* generate_render_tree(HTMLElement* element, RenderBox* parent, float _width)
{
  RenderBox* render_box = new RenderBox(element);
  render_box->set_parent(parent);
  render_box->layout(_width);

  float accumulated_height = 0.f;
  for (HTMLElement* child : element->child_elements())
  {
    if (child->type() == HTMLElementType::TextType)
    {
      Text* text_element = dynamic_cast<Text*>(child);
      if (text_element == nullptr)
        continue;

      RenderBoxText* text_render = new RenderBoxText(text_element);
      accumulated_height += text_render->get_height();

      render_box->add_child(text_render);
    }
    else
    {
      RenderBox* child_render_box = generate_render_tree(child, render_box, render_box->get_width());
      if (child_render_box->get_type() != RenderBoxType::Inline)
        accumulated_height += child_render_box->get_height();

      render_box->add_child(child_render_box);
    }
  }

  float height = accumulated_height + element->style.margin_top + element->style.padding_top + 
                                      element->style.margin_bottom + element->style.padding_bottom;
  render_box->set_height(height);

  return render_box;
}

}
