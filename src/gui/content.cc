#include "content.h"

Gtk::Label* Content::compile_p_tag(const Tag& tag)
{
	auto label = Gtk::make_managed<Gtk::Label>(tag.text());

	label->set_xalign(0.0f);
	label->set_line_wrap(true);
	label->set_selectable(true);
	return label;
}

Content::Content()
	: m_content(Gtk::ORIENTATION_VERTICAL)
{
	std::filesystem::path path = std::filesystem::path("/home/david/workspace/raptor/src/html/examples/index.html");

	HTMLDocument html = HTMLDocument();
	html.load_from_file(path);

	add(m_content);

	auto ps = html.body.tag(P_TOKEN);
	for (int i = 0; i < ps.size(); ++i)
	{
		auto label = compile_p_tag(ps[i]);
		m_content.add(*label);
	}
}
