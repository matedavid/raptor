#pragma once

#include <iostream>
#include <gtkmm.h>

#include "content.h"
#include "tab.h"

class Browser : public Gtk::Window
{
private:
	std::string m_title	= "Raptor Browser";

public:
	Gtk::Box m_container;
	Gtk::Notebook m_notebook;
	Gtk::Entry m_search_bar;
	Gtk::Button m_search_button;

	Content content;

private:
	void search_bar_button_press();

public:
	Browser();
	void new_tab();
};
