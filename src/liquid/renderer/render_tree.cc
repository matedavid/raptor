#include "render_tree.h"

namespace liquid {

float generate_text_tree(Text* text, RenderBox* parent, float width)
{
  RenderBoxText* render_box_text = new RenderBoxText(text, parent);

  float text_height = 0.;
  while (render_box_text->get_width() > width)
  {
    std::string split_content = render_box_text->split_content(width);

    parent->add_child(render_box_text);
    text_height += render_box_text->get_height();

    render_box_text = new RenderBoxText(text, parent);
    render_box_text->set_content(split_content);
  }
  parent->add_child(render_box_text);
  text_height += render_box_text->get_height();

  return text_height;
}

RenderBox* generate_render_tree(HTMLElement* element, RenderBox* parent, float width)
{
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
    // Special treatment for text
    if (child->type() == HTMLElementType::TextType)
    {
      Text* text = dynamic_cast<Text*>(child);
      if (text == nullptr)
        continue;

      accumulated_height += generate_text_tree(text, render_box, render_box->get_width());
      continue;
    }

    RenderBox* render_box_child = generate_render_tree(child, render_box, render_box->get_width());
    if (render_box_child->get_display_type() != RenderBoxDisplayType::Inline)
      accumulated_height += render_box_child->get_height();

    render_box->add_child(render_box_child);
  }

  render_box->compute_height(accumulated_height);

  return render_box;
}

}
