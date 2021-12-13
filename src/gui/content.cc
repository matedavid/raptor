#include "content.h"

void Content::anchor_clicked(Gtk::LinkButton* self)
{
	std::cout << "Anchor clicked with href: " << self->get_uri() << std::endl;
}

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

void Content::render_from_file(const std::string &path)
{
	// Clear content
	// TODO: Probably big memory leak, not deleting all children???
	for (auto child : m_content.get_children())
		m_content.remove(*child);

	liquid::HTMLDocument document;
	document.from_file(path);
	title = document.title;

	liquid::RenderConfig config;
	liquid::RenderInfo info;
	liquid::render(document.body, &m_content, config, info);

	// Apply anchor element clicked function to every <a> element in the content tree
	for (Gtk::LinkButton* link_button : info.anchor_elements)
	{
		link_button->signal_clicked().connect(sigc::bind<Gtk::LinkButton*>(sigc::mem_fun(*this, &Content::anchor_clicked), link_button));
	}

	m_content.show();
	show_all_children();

	// DEBUG
	//liquid::print_html_element(document.body, 0);
}
