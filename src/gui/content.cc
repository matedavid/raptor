#include "content.h"

Content::Content()
	: m_content(Gtk::ORIENTATION_VERTICAL)
{
	add(m_content);

	// Change background color to white background and black widgets
	m_content.override_background_color(Gdk::RGBA("white"));
	m_content.override_color(Gdk::RGBA("black"));

	m_content.show();
	show_all_children();
}

void Content::render_from_file(const std::string& path)
{
	liquid::HTMLDocument document;
	document.from_file(path);

	liquid::print_html_element(document.html, 0);

	auto rendered_result = liquid::render(document.body);
	m_content.add(*rendered_result);

	/*
	std::vector<liquid::HTMLElement*> p_elements = document.html->get_elements_by_tag_name("p");
	for (auto element : p_elements)
	{
		liquid::HTMLParagraphElement* p_element = dynamic_cast<liquid::HTMLParagraphElement*>(element);
		if (p_element == nullptr)
		{
			std::cout << "ERROR: Error converting HTMLElement to ParagraphElement" << std::endl;
			continue;
		}

		auto rendered_p = liquid::render_p_tag(p_element);
		m_content.add(*rendered_p);
	}
	*/
} 
