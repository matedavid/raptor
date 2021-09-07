#pragma once

#include <iostream>
#include <gtkmm.h>

class Content : public Gtk::ScrolledWindow
{
public:
	Content();

protected:
	Gtk::Box m_content;

};
