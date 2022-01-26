#include "render.h"

namespace liquid {

static void set_render_config(RenderConfig& config, const HTMLElement* element)
{
  if (element->type() == HTMLElementType::TextType)
    return;

  if (element->contains_style("text-decoration"))
  {
    std::vector<std::string> text_decoration_values = element->get_style_property_value("text-decoration");
    std::string value = text_decoration_values[0];
    if (value == "none")
      config.text_underline = Pango::Underline::UNDERLINE_NONE;
    else if (value == "underline")
      config.text_underline = Pango::Underline::UNDERLINE_SINGLE;
    else if (value == "overline")
      config.text_overline  = Pango::Overline::OVERLINE_SINGLE;

    if (text_decoration_values.size() > 1)
    {
      std::string color = text_decoration_values[1];
      Gdk::RGBA rgba = Gdk::RGBA(color);

      config.text_decoration_color.set_red(rgba.get_red_u());
      config.text_decoration_color.set_green(rgba.get_green_u());
      config.text_decoration_color.set_blue(rgba.get_blue_u());
    }
  }

  if (element->contains_style("text-decoration-color"))
  {
    std::string value = element->get_style_property_value("text-decoration-color")[0];
    Gdk::RGBA rgba = Gdk::RGBA(value);

    config.text_decoration_color.set_red(rgba.get_red_u());
    config.text_decoration_color.set_green(rgba.get_green_u());
    config.text_decoration_color.set_blue(rgba.get_blue_u());
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

static void apply_common_style(RenderBox* box, HTMLElement* element)
{
  // Apply margin
  box->outer_box->set_margin_top(element->style.margin_top);
  box->outer_box->set_margin_bottom(element->style.margin_bottom);
  box->outer_box->set_margin_left(element->style.margin_left);
  box->outer_box->set_margin_right(element->style.margin_right);

  // Apply padding
  box->inner_box->set_margin_top(element->style.padding_top);
  box->inner_box->set_margin_bottom(element->style.padding_bottom);
  box->inner_box->set_margin_left(element->style.padding_left);
  box->inner_box->set_margin_right(element->style.padding_right);

  // Apply background color
  box->outer_box->override_background_color(Gdk::RGBA(element->style.background_color));
}

RenderBox* render_body_tag(HTMLBodyElement* body_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(body_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, body_element);

  return box;
}

RenderBox* render_div_tag(HTMLDivElement* div_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(div_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, div_element);

  return box;
}

RenderBox* render_p_tag(HTMLParagraphElement* p_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(p_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, p_element);

  return box;
}

void anchor_clicked()
{
  std::cout << "Anchor clicked..." << std::endl;
}

RenderBox* render_a_tag(HTMLAnchorElement* a_element, const RenderConfig& config, RenderInfo& info)
{
  Gtk::Box* outer_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);
  Gtk::Box* inner_box = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

  Gtk::LinkButton* link_button = Gtk::make_managed<Gtk::LinkButton>();
  outer_box->pack_start(*link_button);
  link_button->add(*inner_box);

  outer_box->show();
  link_button->show();
  inner_box->show();

  RenderBox* box = new RenderBox{
    outer_box: outer_box,
    inner_box: inner_box,
    element_value: a_element->element_value
  };
  apply_common_style(box, a_element);

  if (a_element->contains_style("color"))
  {
    std::string color_value = a_element->get_style_property_value("color")[0];
    Gdk::RGBA color = Gdk::RGBA(color_value);
    link_button->override_color(color);
  }

  if (a_element->contains_attribute("href"))
    link_button->set_uri(a_element->get_attribute("href")->value());
  else
    link_button->set_uri("");

  link_button->set_use_underline(false);
  info.anchor_elements.push_back(link_button);

  return box;
}

RenderBox* render_hx_tag(HTMLHeadingElement* h_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(h_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, h_element);

  return box;
}

RenderBox* render_span_tag(HTMLSpanElement* span_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(span_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, span_element);

  return box;
}

RenderBox* render_em_tag(HTMLEmphasisElement* em_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(em_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, em_element);

  return box;
}

RenderBox* render_i_tag(HTMLItalicizedElement* i_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(i_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, i_element);

  return box;
}

RenderBox* render_strong_tag(HTMLStrongElement* strong_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(strong_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, strong_element);

  return box;
}

RenderBox* render_ol_tag(HTMLOrderedListElement* ol_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(ol_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, ol_element);

  return box;
}

RenderBox* render_ul_tag(HTMLUnorderedListElement* ul_element, const RenderConfig& config)
{
  RenderBox* box = new_render_box(ul_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, ul_element);

  return box;
}

RenderBox* render_li_tag(HTMLListItemElement* li_element, Gtk::Box* parent, RenderConfig& config)
{
  RenderBox* box = new_render_box(li_element->element_value, Gtk::ORIENTATION_HORIZONTAL);
  apply_common_style(box, li_element);

  if (not config.list)
  {
    config.list = true;
    config.list_type = 0;
  }

  if (config.list_type == 0)
  {
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>("· "); // TODO: Fix this, use something different to "*" and push it to the left a bit, and match font-size to actual text
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

RenderBox* render_img_tag(HTMLImageElement* img_element, Gtk::Box* parent, const RenderConfig& config)
{
  RenderBox* box = new_render_box(img_element->element_value, Gtk::ORIENTATION_VERTICAL);
  apply_common_style(box, img_element);

  std::string src = img_element->src();
  Glib::RefPtr<Gdk::Pixbuf> pixbuf_img = Gdk::Pixbuf::create_from_file(src);

  Gtk::Image* image = Gtk::make_managed<Gtk::Image>(pixbuf_img);
  image->set_halign(Gtk::Align::ALIGN_START);
  image->set_valign(Gtk::Align::ALIGN_START);

  box->inner_box->pack_start(*image, false, false);

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

  // Font Description
  Pango::FontDescription font_description = Pango::FontDescription();

  font_description.set_size(text->style.font_size*PANGO_SCALE); // font-size
  font_description.set_family(text->style.font_family);         // font-family

  // font-weight
  if (text->style.font_weight.find_first_not_of("0123456789") == std::string::npos)
  {
    int font_weight_num;
    std::stringstream ss;
    ss << text->style.font_weight;
    ss >> font_weight_num;

    font_description.set_weight(Pango::Weight(font_weight_num));
  }
  else
  {
    if (text->style.font_weight == "normal") 
      font_description.set_weight(Pango::Weight::WEIGHT_NORMAL);
    else if (text->style.font_weight == "bold")
      font_description.set_weight(Pango::Weight::WEIGHT_BOLD);
  }

  // font-style
  if (text->style.font_style == "normal")
    font_description.set_style(Pango::Style::STYLE_NORMAL);
  else if (text->style.font_style == "italic")
    font_description.set_style(Pango::Style::STYLE_ITALIC);
  else if (text->style.font_style == "oblique")
    font_description.set_style(Pango::Style::STYLE_OBLIQUE);

  Pango::AttrFontDesc font_description_attr = Pango::Attribute::create_attr_font_desc(font_description);

  // Text decoration
  Pango::AttrInt text_decoration_underline_attr = Pango::Attribute::create_attr_underline(config.text_underline);
	Pango::AttrInt text_decoration_overline_attr  = Pango::Attribute::create_attr_overline(config.text_overline);

  Pango::AttrColor text_decoration_underline_color = Pango::Attribute::create_attr_underline_color(
    config.text_decoration_color.get_red(),
    config.text_decoration_color.get_green(),
    config.text_decoration_color.get_blue()
  );
  Pango::AttrColor text_decoration_overline_color = Pango::Attribute::create_attr_overline_color(
    config.text_decoration_color.get_red(),
    config.text_decoration_color.get_green(),
    config.text_decoration_color.get_blue()
  );

  attr_list.insert(font_description_attr);
  if (config.text_underline != Pango::Underline::UNDERLINE_NONE)
  {
    attr_list.insert(text_decoration_underline_attr);
    attr_list.insert(text_decoration_underline_color);
  }
  else if (config.text_overline != Pango::Overline::OVERLINE_NONE)
  {
    attr_list.insert(text_decoration_overline_attr);
    attr_list.insert(text_decoration_overline_color);
  }

  // Add attributes to label
  label->set_attributes(attr_list);

  return label;
}

void render(HTMLElement* element, Gtk::Box* parent, RenderConfig config, RenderInfo& info)
{
  // Special case where element is text
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

  // Renderer for every element
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
  else if (element->element_value == "a")
  {
    HTMLAnchorElement* a_element = dynamic_cast<HTMLAnchorElement*>(element);
    if (a_element == nullptr)
      return;
    rendered_element = render_a_tag(a_element, config, info);
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
  else if (element->element_value == "em")
  {
    HTMLEmphasisElement* em_element = dynamic_cast<HTMLEmphasisElement*>(element);
    if (em_element == nullptr)
      return;
    rendered_element = render_em_tag(em_element, config);
  }
  else if (element->element_value == "i")
  {
    HTMLItalicizedElement* i_element = dynamic_cast<HTMLItalicizedElement*>(element);
    if (i_element == nullptr)
      return;
    rendered_element = render_i_tag(i_element, config);
  }
  else if (element->element_value == "strong")
  {
    HTMLStrongElement* strong_element = dynamic_cast<HTMLStrongElement*>(element);
    if (strong_element == nullptr)
      return;
    rendered_element = render_strong_tag(strong_element, config);
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
  else if (element->element_value == "img")
  {
    HTMLImageElement* img_element = dynamic_cast<HTMLImageElement*>(element);
    if (img_element == nullptr)
      return;
    rendered_element = render_img_tag(img_element, parent, config);
  }

  parent->pack_start(*rendered_element->outer_box, false, false);

  std::vector<HTMLElement*> children = element->child_elements();
  for (HTMLElement* child : children)
  {
    render(child, rendered_element->inner_box, config, info);
  }
}

}
