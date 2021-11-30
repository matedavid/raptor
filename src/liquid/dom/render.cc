#include "render.h"

namespace liquid {

static void set_render_config(RenderConfig& config, const HTMLElement* element)
{
  if (element->type() == HTMLElementType::TextType)
    return;

  // Common style properties
  if (element->contains_style("font-size"))
  {
    StyleNumber parsed_font_size = parse_number_value(element->get_style_property_value("font-size")[0]);
    if (parsed_font_size.type == ParseNumberType::Em)
      config.font_size *= parsed_font_size.value;
    else 
      config.font_size = parsed_font_size.value;
  }
  if (element->contains_style("font-weight"))
  {
    std::string value = element->get_style_property_value("font-weight")[0];
    // Check fi string is number
    if (value.find_first_not_of("0123456789") == std::string::npos)
    {
      int font_weight_num;
      std::stringstream ss;
      ss << value;
      ss >> font_weight_num;

      config.font_weight = Pango::Weight(font_weight_num);
    }
    else
    {
      if (value == "normal") 
        config.font_weight = Pango::Weight::WEIGHT_NORMAL;
      else if (value == "bold")
        config.font_weight = Pango::Weight::WEIGHT_BOLD;
    }
  }


  // Depending on element_value
  if (element->element_value == "ul")
  {
    config.list = true;
    config.list_type = 0;
  }
  else if (element->element_value == "ol")
  {
    config.list = true;
    config.list_type = 1;
  }

}


static void apply_common_style(RenderBox* box, HTMLElement* element, const RenderConfig& config)
{
  if (element->contains_style("margin"))
  {
    add_margin_style(box->outer_box, element, config);
  }
  if (element->contains_style("margin-top") or 
      element->contains_style("margin-right") or
      element->contains_style("margin-bottom") or
      element->contains_style("margin-left"))
  {
    add_margin_side_style(box->outer_box, element, config);
  }
  if (element->contains_style("padding"))
  {
    add_padding_style(box->inner_box, element, config);
  }
  if (element->contains_style("padding-top") or 
      element->contains_style("padding-right") or
      element->contains_style("padding-bottom") or
      element->contains_style("padding-left"))
  {
    add_padding_side_style(box->inner_box, element, config);
  }
  if (element->contains_style("background-color"))
  {
    add_background_color_style(box->outer_box, element);
  }
}

RenderBox* render_body_tag(HTMLBodyElement* body_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(body_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, body_element, config);

  return box;
}

RenderBox* render_div_tag(HTMLDivElement* div_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(div_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, div_element, config);

  return box;
}

RenderBox* render_p_tag(HTMLParagraphElement* p_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(p_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, p_element, config);

  return box;
}

RenderBox* render_hx_tag(HTMLHeadingElement* h_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(h_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, h_element, config);

  return box;
}

RenderBox* render_span_tag(HTMLSpanElement* span_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(span_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, span_element, config);

  return box;
}

RenderBox* render_ol_tag(HTMLOrderedListElement* ol_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(ol_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, ol_element, config);

  return box;
}

RenderBox* render_ul_tag(HTMLUnorderedListElement* ul_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(ul_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, ul_element, config);

  return box;
}

RenderBox* render_li_tag(HTMLListItemElement* li_element, Gtk::Box* parent, RenderConfig& config)
{
  RenderBox* box = new_render_box(li_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, li_element, config);

  if (not config.list)
  {
    config.list = true;
    config.list_type = 0;
  }

  if (config.list_type == 0)
  {
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>("* ");
    box->inner_box->pack_start(*label, false, false);
  }
  else if (config.list_type == 1)
  {
    // Get the number of the li element by taking into account the position of the 
    // element in relation to it's sibilings
    int number = 1;
    for (HTMLElement* sibiling : li_element->parent_element()->child_elements())
    {
      if (li_element == sibiling)
        break;
      ++number;
    }

    std::string num_marker = std::to_string(number) + ". ";

    Gtk::Label* label = Gtk::make_managed<Gtk::Label>(num_marker);
    box->inner_box->pack_start(*label, false, false);
  }

  return box;
}

Gtk::Label* render_text(Text* text, RenderConfig& config)
{
  Gtk::Label* label = Gtk::make_managed<Gtk::Label>();
  label->set_text(text->content());
  label->set_selectable(true);
	label->set_line_wrap(true);
	label->set_xalign(0.0f);
  label->set_padding(0, 0);
  
  Pango::AttrList attr_list = Pango::AttrList();

  // Font size
  Pango::AttrInt font_size_attr = Pango::Attribute().create_attr_size_absolute(config.font_size*PANGO_SCALE*1.1); // TODO: Check SCALE value

  // Font weight
  Pango::AttrInt font_weight_attr = Pango::Attribute().create_attr_weight(config.font_weight);

  // Font family 
  Pango::AttrFontDesc font_description_attr = Pango::Attribute().create_attr_font_desc(Pango::FontDescription("Times New Roman"));

  attr_list.insert(font_size_attr);
  attr_list.insert(font_description_attr);
  attr_list.insert(font_weight_attr);

  // Add attribytes to label
  label->set_attributes(attr_list);

  return label;
}

void render(HTMLElement* element, Gtk::Box* parent, RenderConfig config)
{
  // Caso especial donde elemento es texto
  if (element->type() == HTMLElementType::TextType)
  {
    Text* text = dynamic_cast<Text*>(element);
    if (text == nullptr)
      return;
    
    Gtk::Label* label = render_text(text, config);
    parent->pack_start(*label, false, false);
    return;
  }

  set_render_config(config, element);

  // Renderizado para cada elemento
  RenderBox* rendered_element;
  if (element->element_value == "body")
  {
    HTMLBodyElement* body_element = dynamic_cast<HTMLBodyElement*>(element);
    if (body_element == nullptr) 
      return;
    rendered_element = render_body_tag(body_element, config);
  }
  else if (element->element_value == "div")
  {
    HTMLDivElement* div_element = dynamic_cast<HTMLDivElement*>(element);
    if (div_element == nullptr)
      return;
    rendered_element = render_div_tag(div_element, config);
  }
  else if (element->element_value == "p")
  {
    HTMLParagraphElement* p_element = dynamic_cast<HTMLParagraphElement*>(element);
    if (p_element == nullptr)
      return;
    rendered_element = render_p_tag(p_element, config);
  }
  else if (element->element_value == "h1" or element->element_value == "h2" or element->element_value == "h3" or
           element->element_value == "h4" or element->element_value == "h5" or element->element_value == "h6")
  {
    HTMLHeadingElement* h_element = dynamic_cast<HTMLHeadingElement*>(element);
    if (h_element == nullptr)
      return;
    rendered_element = render_hx_tag(h_element, config);
  }
  else if (element->element_value == "span")
  {
    HTMLSpanElement* span_element = dynamic_cast<HTMLSpanElement*>(element);
    if (span_element == nullptr)
      return;
    rendered_element = render_span_tag(span_element, config);
  }
  else if (element->element_value == "ol")
  {
    HTMLOrderedListElement* ol_element = dynamic_cast<HTMLOrderedListElement*>(element);
    if (ol_element == nullptr)
      return;
    rendered_element = render_ol_tag(ol_element, config);
  }
  else if (element->element_value == "ul")
  {
    HTMLUnorderedListElement* ul_element = dynamic_cast<HTMLUnorderedListElement*>(element);
    if (ul_element == nullptr)
      return;
    rendered_element = render_ul_tag(ul_element, config);
  }
  else if (element->element_value == "li")
  {
    HTMLListItemElement* li_element = dynamic_cast<HTMLListItemElement*>(element);
    if (li_element == nullptr)
      return;
    rendered_element = render_li_tag(li_element, parent, config);
  }

  parent->pack_start(*rendered_element->outer_box, false, false);

  std::vector<HTMLElement*> children = element->child_elements();
  for (HTMLElement* child : children)
  {
    render(child, rendered_element->inner_box, config);
  }
}

}
