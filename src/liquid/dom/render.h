#pragma once

#include <iostream>
#include <vector>

#include <gtkmm.h>

#include "liquid/html/html_document.h"
#include "liquid/html/html_paragraph_element.h"

#include "liquid/dom/style.h"

namespace liquid {

// Layout that "emulates" the Box Layout model in CSS
// Used to represent rendered elements in DOM
struct RenderBox
{
  Gtk::Box* outer_box;
  Gtk::Box* inner_box;
  std::string element_value;
};

RenderBox* new_render_box(const std::string& element_value, Gtk::Orientation orientation);

RenderBox* render_body_tag(HTMLBodyElement* body_element, Gtk::Box* parent);
RenderBox* render_div_tag(HTMLDivElement* div_element);
RenderBox* render_p_tag(HTMLParagraphElement* p_element);
RenderBox* render_span_element(HTMLSpanElement* span_element);
RenderBox* render_ol_tag(HTMLOrderedListElement* ol_element);
RenderBox* render_ul_tag(HTMLUnorderedListElement* ul_element);
RenderBox* render_li_tag(HTMLListItemElement* li_element);

Gtk::Label* render_text(Text* text);

void render(HTMLElement* element, Gtk::Box* parent);

}