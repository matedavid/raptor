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

struct RenderConfig 
{
  // Text
  StyleNumber font_size = {
    16.0,                     // DEFAULT_FONT_SIZE: 16
    ParseNumberType::Px
  };

  // Lists
  bool list;                  // If element is under a list
  unsigned int list_type;     // ul: 0 - ol: 1
  int unordered_list_marker;  // TODO: Not implemented, should decide how to represent markers
};

static void set_render_config(RenderConfig& config, const HTMLElement* element);
static void unset_render_config(RenderConfig& config, const HTMLElement* element);
static RenderBox* new_render_box(const std::string& element_value, Gtk::Orientation orientation);

RenderBox* render_body_tag(HTMLBodyElement* body_element, Gtk::Box* parent);
RenderBox* render_div_tag(HTMLDivElement* div_element);
RenderBox* render_p_tag(HTMLParagraphElement* p_element);
RenderBox* render_span_tag(HTMLSpanElement* span_element);
RenderBox* render_ol_tag(HTMLOrderedListElement* ol_element);
RenderBox* render_ul_tag(HTMLUnorderedListElement* ul_element);
RenderBox* render_li_tag(HTMLListItemElement* li_element, Gtk::Box* parent, RenderConfig* config);

Gtk::Label* render_text(Text* text, RenderConfig& config);

void render(HTMLElement* element, Gtk::Box* parent, RenderConfig config);

}