#include "tab.h"

Tab::Tab()
  : m_toolbar(), m_search_bar(), m_search_button(), m_new_tab_button()
{
  // New Tab button
  m_new_tab_button.set_label("+");
  m_new_tab_button.set_margin_top(10);
  m_new_tab_button.set_margin_bottom(10);
  m_new_tab_button.set_margin_left(10);
  m_new_tab_button.set_margin_right(10);

  // Search button
  m_search_button.set_label("Search");
  m_search_button.set_margin_top(10);
  m_search_button.set_margin_bottom(10);
  m_search_button.set_margin_left(10);
  m_search_button.set_margin_right(10);

  m_search_button.signal_clicked().connect([this] { this->search_bar_button_pressed(); });

  // Search bar
	m_search_bar.set_margin_top(10);
	m_search_bar.set_margin_bottom(10);
	m_search_bar.set_margin_left(10);
	m_search_bar.set_margin_right(10);

  m_toolbar.pack_start(m_new_tab_button, false, false);
  m_toolbar.pack_start(m_search_bar);
  m_toolbar.pack_start(m_search_button, false, false);
  pack_start(m_toolbar, false, false);

  //m_content.render_from_file("/home/david/workspace/raptor/examples/index.html");
  pack_start(m_content);

  show_all_children();
}

void Tab::search_bar_button_pressed()
{
	std::string value = m_search_bar.get_text();
	if (not value.empty())
	{
		m_content.render_from_file(value);
	}
}
