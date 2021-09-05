#pragma once

#include <iostream>
#include <gtkmm.h>

#include "html/htmldocument.h"

class Content : public Gtk::ScrolledWindow
{
private:
	Gtk::Label* compile_p_tag(const Tag& tag);

public:
	Content();

protected:
	Gtk::Box m_content;

};
