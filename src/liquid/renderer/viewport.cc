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

Viewport::Viewport(HTMLElement* body)
{
  const float WIDTH = 960.f;

  render_tree = construct_render_tree(body, nullptr);
  //Dimensions dims = render_tree->compute_dimensions(WIDTH);

  render_tree->layout({
    .xref=0.f,
    .yref=0.f,
    .margin_top_applied=0.f,
    .container_width=WIDTH
  });

  render_tree->print(0);
  //std::cout << "Total height: " << render_tree.height << std::endl;
}

}
