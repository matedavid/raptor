#pragma once

#include <iostream>
#include <vector>
#include <gtkmm.h>

#include "liquid/html/html_document.h"


class Renderer
{
private:
  Gtk::Box* container;

  Gtk::Box* render_rec(liquid::HTMLElement* element);

public:
  Renderer();

  Gtk::Box* render_p_tag(liquid::HTMLElement* p_element);

  void render(liquid::HTMLDocument* document);
};