#pragma once

#include <iostream>
#include <gtkmm.h>

#include "liquid/dom/render.h"

#include "liquid/html/html_document.h"
#include "liquid/html/html_paragraph_element.h"

class Content : public Gtk::ScrolledWindow
{
protected:
	Gtk::Box m_content;

public:
	std::string title;

private:
	void anchor_clicked(Gtk::LinkButton* self);

public:
	Content();

	void render_from_file(const std::string& path);

};
