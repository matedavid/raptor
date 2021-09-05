#pragma once

#include <iostream>
#include <gtkmm.h>

#include "content.h"

#include "html/htmldocument.h"

class Browser : public Gtk::Window
{
public:
	Browser();

	Gtk::Box m_container;
	Gtk::Entry m_search_bar;

	Content content;
};
