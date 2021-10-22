#pragma once

#include <iostream>
#include <vector>

#include <gtkmm.h>

#include "liquid/html/html_document.h"
#include "liquid/html/html_paragraph_element.h"

namespace liquid {

Gtk::Box* render_p_tag(liquid::HTMLParagraphElement* p_element);

}