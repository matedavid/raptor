#pragma once

#include <iostream>
#include <gtkmm.h>

#include "html/htmldocument.h"

class Browser : public Gtk::Window
{
public:
	Browser();

	Gtk::ScrolledWindow m_scrolled_window;
	Gtk::Grid m_grid;
};
