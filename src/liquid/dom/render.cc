#include "render.h"

namespace liquid {

Gtk::Box* render_p_tag(liquid::HTMLParagraphElement* p_element)
{
  // TODO: Make this dependant on the style attribute of the element
  int font_size = 12;

  auto box = Gtk::make_managed<Gtk::Box>();
  box->set_margin_top(1*font_size); // Because of 1em
  box->set_margin_bottom(1*font_size);
  box->set_margin_left(0);
  box->set_margin_right(0);

  /*
  std::vector<HTMLElement*> elems = p_element->get_elements_by_tag_name("text");
  std::cout << "Number text elements: " << elems.size() << std::endl;
  */
  /*
  for (auto p : elems)
  {
    Text* t = dynamic_cast<Text*>(p);
    if (t == nullptr)
      continue;
    std::cout << "|" << t->content() << "|" << std::endl;
  }
  */

  /*
  auto label = Gtk::make_managed<Gtk::Label>();
  label->set_text(content);
  label->set_selectable(true);
	label->set_line_wrap(true);
	label->set_xalign(0.0f);

  Pango::AttrList attr_list = Pango::AttrList();
  Pango::AttrInt font_size_attr = Pango::Attribute().create_attr_size_absolute(font_size*PANGO_SCALE);

  attr_list.insert(font_size_attr);
  label->set_attributes(attr_list);
 
  box->add(*label);

  label->show();
  */

  box->show();

  return box;
}

}