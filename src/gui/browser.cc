#include "browser.h"

Browser::Browser()
	: m_container(Gtk::ORIENTATION_VERTICAL), m_search_bar(), m_search_button()
{
	set_title(m_title);
	set_default_size(800, 600);

	add(m_container);

	m_search_bar.set_margin_top(10);
	m_search_bar.set_margin_bottom(10);
	m_search_bar.set_margin_left(10);
	m_search_bar.set_margin_right(10);

	m_search_button.set_label("Search");
	m_search_button.signal_clicked().connect([this] { this->search_bar_button_press(); } );

	Gtk::Box* m_toolbar_container = Gtk::make_managed<Gtk::Box>(Gtk::ORIENTATION_HORIZONTAL);

	m_toolbar_container->pack_start(m_search_bar);
	m_toolbar_container->add(m_search_button);

	m_container.add(*m_toolbar_container);

	Content* page1 = new Content();
	page1->render_from_file("/home/david/workspace/raptor/examples/index.html");

	m_notebook.append_page(*page1, "Example HTML");

	//m_container.pack_start(content);
	//content.render_from_file("/home/david/workspace/raptor/examples/index.html");

	m_container.pack_start(m_notebook);

	m_container.show();
	show_all_children();
}

void Browser::search_bar_button_press()
{
	std::string value = m_search_bar.get_text();
	if (not value.empty())
	{
		content.render_from_file(value);
	}
}