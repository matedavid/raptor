#include "content.h"

Content::Content()
	: m_content(Gtk::ORIENTATION_VERTICAL)
{
	add(m_content);

	m_content.show();
	show_all_children();
}

void Content::render_from_file(const std::string& path)
{
	liquid::HTMLDocument document;
	document.from_file(path);

	Renderer renderer;
	std::vector<liquid::HTMLElement*> p_elements = document.body->get_elements_by_tag_name("p");

	for (auto element : p_elements)
	{
		Gtk::Box* rendered_p = renderer.render_p_tag(element);
		m_content.add(*rendered_p);
	}
}