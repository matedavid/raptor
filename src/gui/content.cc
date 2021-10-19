#include "content.h"

Gtk::Label* get_label(liquid::Text* text)
{
	Gtk::Label* label = Gtk::make_managed<Gtk::Label>(text->content());

	// Wraps text to new line if it's horizontally bigger than the window
	label->set_line_wrap(true);
	// Aligns the widget to the left
	label->set_xalign(0.0f);

	return label;
}


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

	std::vector<liquid::HTMLElement*> text_elements = document.body->get_elements_by_tag_name("text");
	for (liquid::HTMLElement* text_element : text_elements)
	{
		if (text_element == nullptr)
			continue;

		liquid::Text* text = dynamic_cast<liquid::Text*>(text_element);
		Gtk::Label* label = get_label(text);
		m_content.add(*label);

		label->show();
	}
}