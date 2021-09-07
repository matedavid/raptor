#pragma once

#include <iostream>
#include <gtkmm.h>

#include "content.h"

class Browser : public Gtk::Window
{
public:
	Browser();

	Gtk::Box m_container;
	Gtk::Entry m_search_bar;

	Content content;
};
