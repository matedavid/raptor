#include "browser.h"

Browser::Browser()
	: m_container(Gtk::ORIENTATION_VERTICAL), m_search_bar(), m_search_button()
{
	set_title(m_title);
	set_default_size(800, 600);

	add(m_container);

	//m_search_button.signal_clicked().connect([this] { this->search_bar_button_press(); } );

	/*
	Tab* tab1 = new Tab();
	tab1->m_search_button.signal_clicked().connect([this] { this->search_bar_button_press(); });
	tab1->m_new_tab_button.signal_clicked().connect([this] { this->new_tab(); });
	m_notebook.append_page(*tab1, "Tab 1");
	*/

	// First tab
	new_tab();

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

void Browser::new_tab()
{
	Tab* tab = new Tab();
	tab->m_content.render_from_file("/home/david/workspace/raptor/src/gui/templates/new_tab.html");

	//tab->m_search_button.signal_clicked().connect([this] { this->search_bar_button_press(); });
	tab->m_new_tab_button.signal_clicked().connect([this] { this->new_tab(); });

	m_notebook.append_page(*tab, tab->m_content.title);
	tab->show();
}
