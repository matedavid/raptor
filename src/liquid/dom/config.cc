#include "config.h"

namespace liquid {

RenderBox* new_render_box(const std::string& element_value, Gtk::Orientation orientation)
{
  Gtk::Box* outer_box = Gtk::make_managed<Gtk::Box>(orientation);
  Gtk::Box* inner_box = Gtk::make_managed<Gtk::Box>(orientation);
  outer_box->pack_start(*inner_box);

  return new RenderBox{
		.outer_box = outer_box,
		.inner_box = inner_box,
		.element_value = element_value
  };
}

}
