#pragma once

#include <iostream>
#include <gtkmm.h>

#include "gui/test_renderer.h"

#include "liquid/html/html_document.h"

class Content : public Gtk::ScrolledWindow
{
protected:
	Gtk::Box m_content;

public:
	Content();

	void render_from_file(const std::string& path);

};
