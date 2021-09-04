#include "browser.h"

Browser::Browser()
	: m_scrolled_window(), m_grid()
{
	std::filesystem::path path = std::filesystem::path("/home/david/workspace/raptor/src/html/examples/index.html");

	HTMLDocument html = HTMLDocument();
	html.load_from_file(path);

	set_title(html.title);
	set_default_size(800, 480);

	add(m_scrolled_window);

	m_scrolled_window.add(m_grid);

	std::vector<Tag> ps = html.body.tag(P_TOKEN);
	for (int i = 0; i < ps.size(); ++i)
	{
		std::string str = ps[i].text();
		auto label = Gtk::make_managed<Gtk::Label>(str);
		m_grid.attach(*label, 0, i);
	}

	show_all_children();
}
