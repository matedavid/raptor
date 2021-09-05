#include "browser.h"

Gtk::Label* label_from_tag(const Tag& tag)
{
	auto label = Gtk::make_managed<Gtk::Label>();
	label->set_text(tag.text());
	// Wraps text to new line if it's horizontally bigger than the window
	label->set_line_wrap(true);
	// Aligns the widget to the left
	label->set_xalign(0.0f);
	return label;
}

Browser::Browser()
	: m_container(Gtk::ORIENTATION_VERTICAL), m_search_bar(), content()
{
	std::string title = "Raptor Browser";

	set_title(title);
	set_default_size(800, 480);

	add(m_container);

	m_search_bar.set_margin_top(10);
	m_search_bar.set_margin_bottom(10);
	m_search_bar.set_margin_left(10);
	m_search_bar.set_margin_right(10);

	m_container.add(m_search_bar);
	m_container.pack_start(content);

	m_container.show();
	show_all_children();
}
