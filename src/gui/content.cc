#include "content.h"

void Content::anchor_clicked(Gtk::LinkButton* self)
{
	std::string uri = self->get_uri();
	if (uri[0] == '/')
	{
		this->render_from_file(uri);
	}
	else 
	{
		std::filesystem::path relative_path = std::filesystem::path(this->url).parent_path();
		relative_path /= uri;

		this->render_from_file(relative_path.c_str());
	}
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
	url = path;

	liquid::RenderConfig config;
	liquid::RenderInfo info;
	liquid::render(document.body, &m_content, config, info);

	// Apply anchor element clicked function to every <a> element in the content tree
	for (Gtk::LinkButton* link_button : info.anchor_elements)
		link_button->signal_clicked().connect(sigc::bind<Gtk::LinkButton*>(sigc::mem_fun(*this, &Content::anchor_clicked), link_button));

	m_content.show();
	show_all_children();

	// DEBUG
	liquid::print_html_element(document.body, 0);


	auto fsss = document.body->get_elements_by_class_name("border");
  if (fsss.size() != 0)
  {
    auto border_example = fsss[0];
    std::cout << "Top: " << border_example->style.border_width[0] << std::endl;
    std::cout << "Right: " << border_example->style.border_width[1] << std::endl;
    std::cout << "Bottom: " << border_example->style.border_width[2] << std::endl;
    std::cout << "Left: " << border_example->style.border_width[3] << std::endl;
  }

	
}
