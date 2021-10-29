#pragma once

#include <iostream>
#include <vector>

#include <gtkmm.h>

#include "liquid/html/html_document.h"
#include "liquid/html/html_paragraph_element.h"

#include "liquid/dom/style.h"

namespace liquid {

Gtk::Box* render_body_tag(HTMLBodyElement* body_element);
Gtk::Box* render_div_tag(HTMLDivElement* div_element);
Gtk::Box* render_p_tag(HTMLParagraphElement* p_element);
Gtk::Box* render_span_element(HTMLSpanElement* span_element);
Gtk::Box* render_text(Text* text);

Gtk::Box* render(HTMLElement* element);

}