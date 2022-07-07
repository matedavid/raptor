#include "viewport.h"

namespace liquid {

RenderBox* Viewport::construct_render_tree(HTMLElement* element, RenderBox* parent)
{
  if (element->type() == HTMLElementType::TextType)
  {
    Text* text = dynamic_cast<Text*>(element);
    RenderBoxText* render_box_text = new RenderBoxText(text, parent);
    return render_box_text;
  }
  else if (element->element_value == "img")
  {
    // TODO: Generate RenderBoxImage + return 
  }

  RenderBox* render_box = new RenderBox(element, parent);
  for (HTMLElement* child : element->child_elements())
  {
    RenderBox* new_child = construct_render_tree(child, render_box);
    render_box->insert_child(new_child);
  }

  return render_box;
}

void Viewport::compute_height(RenderBox* render_box)
{
  render_box->compute_height();
}

Viewport::Viewport(HTMLElement* body)
{
  RenderBox* render_tree = construct_render_tree(body, nullptr);
  render_tree->print(0);
}

}
