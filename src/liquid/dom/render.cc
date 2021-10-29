#include "render.h"

namespace liquid {


Gtk::Box* render_body_tag(HTMLBodyElement* body_element)
{
  auto box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);

  if (body_element->contains_style("margin"))
  {
    add_margin_style(box, body_element);
  }
  if (body_element->contains_style("margin-top") or 
      body_element->contains_style("margin-right") or
      body_element->contains_style("margin-bottom") or
      body_element->contains_style("margin-left"))
  {
    add_margin_side_style(box, body_element);
  }
  if (body_element->contains_style("background-color"))
  {
    add_background_color_style(box, body_element);
  }

  return box;
}

Gtk::Box* render_div_tag(HTMLDivElement* div_element)
{
  auto box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);

  if (div_element->contains_style("margin"))
  {
    add_margin_style(box, div_element);
  }
  if (div_element->contains_style("margin-top") or 
      div_element->contains_style("margin-right") or
      div_element->contains_style("margin-bottom") or
      div_element->contains_style("margin-left"))
  {
    add_margin_side_style(box, div_element);
  }
  if (div_element->contains_style("background-color"))
  {
    add_background_color_style(box, div_element);
  }

  return box;
}

Gtk::Box* render_p_tag(HTMLParagraphElement* p_element)
{
  auto box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

  if (p_element->contains_style("margin"))
  {
    add_margin_style(box, p_element);
  }
  if (p_element->contains_style("margin-top") or 
      p_element->contains_style("margin-right") or
      p_element->contains_style("margin-bottom") or
      p_element->contains_style("margin-left"))
  {
    add_margin_side_style(box,  p_element);
  }
  if (p_element->contains_style("background-color"))
  {
    add_background_color_style(box, p_element);
  }

  return box;
}

Gtk::Box* render_span_tag(HTMLSpanElement* span_element)
{
  auto box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

  if (span_element->contains_style("margin"))
  {
    add_margin_style(box, span_element);
  }
  if (span_element->contains_style("margin-top") or 
      span_element->contains_style("margin-right") or
      span_element->contains_style("margin-bottom") or
      span_element->contains_style("margin-left"))
  {
    add_margin_side_style(box, span_element);
  }
  if (span_element->contains_style("background-color"))
  {
    add_background_color_style(box, span_element);
  }
  return box;
}

Gtk::Box* render_text(Text* text)
{
  Gtk::Box* box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_VERTICAL);

  Gtk::Label* label = Gtk::make_managed<Gtk::Label>();
  label->set_text(text->content());
  label->set_selectable(true);
	label->set_line_wrap(true);
	label->set_xalign(0.0f);
  label->set_padding(0, 0);

  Pango::AttrList attr_list = Pango::AttrList();
  Pango::AttrInt font_size_attr = Pango::Attribute().create_attr_size_absolute(DEFAULT_FONT_SIZE*PANGO_SCALE*PANGO_SCALE_LARGE);
  Pango::AttrFontDesc font_description_attr = Pango::Attribute().create_attr_font_desc(Pango::FontDescription("Times New Roman"));

  attr_list.insert(font_size_attr);
  attr_list.insert(font_description_attr);
  label->set_attributes(attr_list);

  box->pack_start(*label);

  return box;
}

Gtk::Box* render(HTMLElement* element)
{
  Gtk::Box* rendered_element;
  if (element->element_value == "body")
  {
    HTMLBodyElement* body_element = dynamic_cast<HTMLBodyElement*>(element);
    if (body_element == nullptr)
      return nullptr;
    rendered_element = render_body_tag(body_element);
  }
  else if (element->element_value == "div")
  {
    HTMLDivElement* div_element = dynamic_cast<HTMLDivElement*>(element);
    if (div_element == nullptr)
      return nullptr;  
    rendered_element = render_div_tag(div_element);
  }
  else if (element->element_value == "p")
  {
    HTMLParagraphElement* p_element = dynamic_cast<HTMLParagraphElement*>(element);
    if (p_element == nullptr)
      return nullptr; 
    rendered_element = render_p_tag(p_element);
  }
  else if (element->element_value == "span")
  {
    HTMLSpanElement* span_element = dynamic_cast<HTMLSpanElement*>(element);
    if (span_element == nullptr)
      return nullptr;
    rendered_element = render_span_tag(span_element);
  }
  else if (element->type() == HTMLElementType::TextType)
  {
    Text* text = dynamic_cast<Text*>(element);
    if (text == nullptr)
      return nullptr;
    rendered_element = render_text(text);
  }

  /* === Rendering children === */
  std::vector<HTMLElement*> children = element->child_elements();
  for (HTMLElement* e : children)
  {
    Gtk::Box* rendered_child = render(e);
    rendered_element->pack_start(*rendered_child, false, false);
  }

  return rendered_element;
}

}
