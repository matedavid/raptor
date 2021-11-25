#include "content.h"

Content::Content()
	: m_content(Gtk::ORIENTATION_VERTICAL)
{
	add(m_content);

	// Change background color to white and black widgets
	m_content.override_background_color(Gdk::RGBA("white"));
	m_content.override_color(Gdk::RGBA("black"));

	m_content.show();
	show_all_children();
}

void Content::render_from_file(const std::string& path)
{
	liquid::HTMLDocument document;
	document.from_file(path);
	title = document.title;

	liquid::render(document.body, &m_content, liquid::RenderConfig{});

	// DEBUG
	liquid::print_html_element(document.body, 0);
}
