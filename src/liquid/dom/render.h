#pragma once

#include <iostream>
#include <vector>

#include <gtkmm.h>

#include "liquid/html/html_document.h"
#include "liquid/html/html_paragraph_element.h"

#include "liquid/dom/style.h"
#include "liquid/dom/config.h"

namespace liquid {

static void set_render_config(RenderConfig& config, const HTMLElement* element);
static void apply_common_style(RenderBox* box, HTMLElement* element, const RenderConfig& config);

RenderBox* render_body_tag(HTMLBodyElement* body_element, Gtk::Box* parent, const RenderConfig& config);
RenderBox* render_div_tag(HTMLDivElement* div_element, const RenderConfig& config);
RenderBox* render_p_tag(HTMLParagraphElement* p_element, const RenderConfig& config);
RenderBox* render_a_tag(HTMLAnchorElement* a_element, const RenderConfig& config, RenderInfo& info);
RenderBox* render_hx_tag(HTMLHeadingElement* h_element, const RenderConfig& config);
RenderBox* render_span_tag(HTMLSpanElement* span_element, const RenderConfig& config);
RenderBox* render_em_tag(HTMLEmphasisElement* em_element, const RenderConfig& config);
RenderBox* render_i_tag(HTMLItalicizedElement* i_element, const RenderConfig& config);
RenderBox* render_strong_tag(HTMLStrongElement* strong_element, const RenderConfig& config);
RenderBox* render_ol_tag(HTMLOrderedListElement* ol_element, const RenderConfig& config);
RenderBox* render_ul_tag(HTMLUnorderedListElement* ul_element, const RenderConfig& config);
RenderBox* render_li_tag(HTMLListItemElement* li_element, Gtk::Box* parent, RenderConfig& config);
RenderBox* render_img_tag(HTMLImageElement* img_element, Gtk::Box* parent, const RenderConfig& config);

Gtk::Label* render_text(Text* text, RenderConfig& config);

void render(HTMLElement* element, Gtk::Box* parent, RenderConfig config, RenderInfo& info);

}