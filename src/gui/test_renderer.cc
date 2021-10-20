#include "test_renderer.h"

Gtk::Box* Renderer::render_p_tag(liquid::HTMLElement* p_element)
{
  std::vector<liquid::HTMLElement*> text_elements = p_element->get_elements_by_tag_name("text");
  if (text_elements.size() == 0)
    return nullptr;

  liquid::Text* text = dynamic_cast<liquid::Text*>(text_elements[0]);
  if (text == nullptr)
    return nullptr;

  int font_size = 50;

  auto box = Gtk::make_managed<Gtk::Box>();
  box->set_margin_top(1*font_size); // Because of 1em
  box->set_margin_bottom(1*font_size);
  box->set_margin_left(0);
  box->set_margin_right(0);

  auto label = Gtk::make_managed<Gtk::Label>();
  label->set_text(text->content());
  label->set_selectable(true);
	label->set_line_wrap(true);
	label->set_xalign(0.0f);

  Pango::AttrList attr_list = Pango::AttrList();
  Pango::AttrInt font_size_attr = Pango::Attribute().create_attr_size_absolute(font_size*PANGO_SCALE);

  attr_list.insert(font_size_attr);
  label->set_attributes(attr_list);
 
  box->add(*label);

  label->show();
  box->show();

  return box;
}

/*
Gtk::Box* Renderer::render_rec(liquid::HTMLElement* element)
{
  if (element->element_value == "p")
  {
    return render_p_tag(element);
  }
}
*/

Renderer::Renderer()
{
}

void Renderer::render(liquid::HTMLDocument* document)
{
}
