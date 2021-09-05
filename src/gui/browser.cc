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
	: m_container(Gtk::ORIENTATION_VERTICAL), m_scrolled_window(), m_search_bar(), m_content(Gtk::ORIENTATION_VERTICAL)
{
	std::filesystem::path path = std::filesystem::path("/home/david/workspace/raptor/src/html/examples/index.html");

	HTMLDocument html = HTMLDocument();
	html.load_from_file(path);

	set_title(html.title);
	set_default_size(800, 480);

	add(m_container);

	m_search_bar.set_margin_top(10);
	m_search_bar.set_margin_bottom(10);
	m_search_bar.set_margin_left(10);
	m_search_bar.set_margin_right(10);

	m_container.add(m_search_bar);
	m_container.pack_start(m_scrolled_window);

	m_scrolled_window.add(m_content);

	std::vector<Tag> ps = html.body.tag(P_TOKEN);
	for (int i = 0; i < ps.size(); ++i)
	{
		auto label = label_from_tag(ps[i]);
		m_content.add(*label);
	}

	m_container.show();
	show_all_children();
}
