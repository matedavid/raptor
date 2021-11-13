#include "render.h"

namespace liquid {

RenderBox* new_render_box(const std::string& element_value, Gtk::Orientation orientation)
{
  Gtk::Box* outer_box = Gtk::make_managed<Gtk::Box>(orientation);
  Gtk::Box* inner_box = Gtk::make_managed<Gtk::Box>(orientation);
  outer_box->pack_start(*inner_box);

  return new RenderBox{
    outer_box: outer_box,
    inner_box: inner_box,
    element_value: element_value
  };
}

void apply_common_style(RenderBox* box, HTMLElement* element)
{
  if (element->contains_style("margin"))
  {
    add_margin_style(box->outer_box, element);
  }
  if (element->contains_style("margin-top") or 
      element->contains_style("margin-right") or
      element->contains_style("margin-bottom") or
      element->contains_style("margin-left"))
  {
    add_margin_side_style(box->outer_box, element);
  }
  if (element->contains_style("padding"))
  {
    add_padding_style(box->inner_box, element);
  }
  if (element->contains_style("padding-top") or 
      element->contains_style("padding-right") or
      element->contains_style("padding-bottom") or
      element->contains_style("padding-left"))
  {
    add_padding_side_style(box->inner_box, element);
  }
  if (element->contains_style("background-color"))
  {
    add_background_color_style(box->outer_box, element);
  }
}

RenderBox* render_body_tag(HTMLBodyElement* body_element)
{
  RenderBox* box = new_render_box(body_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, body_element);

  return box;
}

RenderBox* render_div_tag(HTMLDivElement* div_element)
{
  RenderBox* box = new_render_box(div_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, div_element);

  return box;
}

RenderBox* render_p_tag(HTMLParagraphElement* p_element)
{
  RenderBox* box = new_render_box(p_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, p_element);

  return box;
}

RenderBox* render_span_tag(HTMLSpanElement* span_element)
{
  RenderBox* box = new_render_box(span_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, span_element);

  return box;
}

Gtk::Label* render_text(Text* text)
{
  Gtk::Label* label = Gtk::make_managed<Gtk::Label>();
  label->set_text(text->content());
  label->set_selectable(true);
	label->set_line_wrap(true);
	label->set_xalign(0.0f);
  label->set_padding(0, 0);
  
  // TODO: This should be dynamic (dependant on style)
  Pango::AttrList attr_list = Pango::AttrList();
  Pango::AttrInt font_size_attr = Pango::Attribute().create_attr_size_absolute(DEFAULT_FONT_SIZE*PANGO_SCALE*PANGO_SCALE_LARGE);
  Pango::AttrFontDesc font_description_attr = Pango::Attribute().create_attr_font_desc(Pango::FontDescription("Times New Roman"));

  attr_list.insert(font_size_attr);
  attr_list.insert(font_description_attr);
  label->set_attributes(attr_list);

  return label;
}

void render(HTMLElement* element, Gtk::Box* parent)
{
  // Caso especial donde elemento es texto
  if (element->type() == HTMLElementType::TextType)
  {
    Text* text = dynamic_cast<Text*>(element);
    if (text == nullptr)
      return;
    
    Gtk::Label* label = render_text(text);
    parent->pack_start(*label, false, false);
    return;
  }

  // Renderizado para cada elemento
  RenderBox* rendered_element;
  if (element->element_value == "body")
  {
    HTMLBodyElement* body_element = dynamic_cast<HTMLBodyElement*>(element);
    if (body_element == nullptr) 
      return;
    rendered_element = render_body_tag(body_element);
  }
  else if (element->element_value == "div")
  {
    HTMLDivElement* div_element = dynamic_cast<HTMLDivElement*>(element);
    if (div_element == nullptr)
      return;
    rendered_element = render_div_tag(div_element);
  }
  else if (element->element_value == "p")
  {
    HTMLParagraphElement* p_element = dynamic_cast<HTMLParagraphElement*>(element);
    if (p_element == nullptr)
      return;
    rendered_element = render_p_tag(p_element);
  }
  else if (element->element_value == "span")
  {
    HTMLSpanElement* span_element = dynamic_cast<HTMLSpanElement*>(element);
    if (span_element == nullptr)
      return;
    rendered_element = render_span_tag(span_element);
  }

  parent->pack_start(*rendered_element->outer_box, false, false);

  std::vector<HTMLElement*> children = element->child_elements();
  for (HTMLElement* child : children)
    render(child, rendered_element->inner_box);
}

}
