#pragma once

#include <iostream>
#include <gtkmm.h>

#include "html/htmldocument.h"

class Browser : public Gtk::Window
{
public:
	Browser();

	Gtk::Box m_container;
	Gtk::Entry m_search_bar;

	Gtk::ScrolledWindow m_scrolled_window;
	Gtk::Box m_content;
};
