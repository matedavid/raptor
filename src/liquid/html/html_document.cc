#include "html_document.h"

namespace liquid {

HTMLDocument::HTMLDocument()
	: head(nullptr), body(nullptr)
{
}

void HTMLDocument::add_css_block(const CSSBlock& css_block)
{
	for (SelectorValue selector : css_block.selector_targets)
	{
		std::vector<HTMLElement*> target_elements;
		if (selector.option == SelectorValueOption::Class)
			target_elements = html->get_elements_by_class_name(selector.value);
		else if (selector.option == SelectorValueOption::ID)
			target_elements = html->get_elements_by_id(selector.value);
		else
			target_elements = html->get_elements_by_tag_name(selector.value);

		for (HTMLElement* element : target_elements)
			element->apply_css(css_block.declarations);
	}
}

void HTMLDocument::from_string(const std::string& content)
{
	Tokenizer tokenizer = Tokenizer();
	tokenizer.tokenize(content);

	HTMLParser parser = HTMLParser();
	html = parser.parse(tokenizer, document_path);

	title = "New Tab";
	std::vector<HTMLElement*> title_elements = html->get_elements_by_tag_name("title");
	if (title_elements.size() != 0) 
	{
		std::vector<HTMLElement*> text_elements = title_elements[0]->get_elements_by_tag_name("text");
		if (text_elements.size() != 0)
		{
			Text* text = dynamic_cast<Text*>(text_elements[0]);
			title = text->content();
		}
	}

	HTMLElement* head_element = html->get_elements_by_tag_name("head")[0];
	if (head_element != nullptr)
		head = dynamic_cast<HTMLHeadElement*>(head_element);

	HTMLElement* body_element = html->get_elements_by_tag_name("body")[0];
	if (body_element != nullptr)
		body = dynamic_cast<HTMLBodyElement*>(body_element);

	/* === TODO: This is a temporal solution === */
	// Default css attributes from file
	CSSParser default_parser;
	std::vector<CSSBlock> default_blocks = default_parser.from_file("/home/david/workspace/raptor/src/default.css");
	for (CSSBlock block : default_blocks)
	{
		add_css_block(block);
	}
	/* ========================================= */

	// Include CSS into HTMLElements
	CSSParser css_parser;

	// Get CSS from Link elements	
	std::vector<HTMLElement*> link_elements = head->get_elements_by_tag_name("link");
	for (HTMLElement* link : link_elements)
	{
		if (link->contains_attribute("rel") and link->contains_attribute("href"))
		{
			if (link->get_attribute("rel")->value() != "stylesheet")
				continue;

			std::string str_path = link->get_attribute("href")->value();

			std::filesystem::path path = document_path.parent_path();
			path /= str_path;

			std::vector<CSSBlock> css_blocks = css_parser.from_file(path);
			for (CSSBlock block : css_blocks)
				add_css_block(block);
		}
	}

	// TODO: Get CSS from style elements
	std::vector<HTMLElement*> style_elements = head->get_elements_by_tag_name("style");
	for (HTMLElement* style : style_elements)
	{
		if (not style->contains_attribute("type") or style->get_attribute("type")->value() != "text/css")
			continue;

		std::vector<HTMLElement*> text_elements = style->get_elements_by_tag_name("text");
		if (text_elements.size() == 0)
			continue;

		Text* text = dynamic_cast<Text*>(text_elements[0]);
		std::string style_string = text->content();

		std::vector<CSSBlock> css_blocks = css_parser.from_string(style_string);
		for (CSSBlock block : css_blocks)
			add_css_block(block);
	}
}

void HTMLDocument::from_file(const std::filesystem::path& file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open())
	{
		std::cout << "Error opening file: " << file_path << std::endl;
		exit(0);
	}
	std::string content( (std::istreambuf_iterator<char>(file) ),
											 (std	::istreambuf_iterator<char>()   ));

	std::string final_content = "";
	for (int i = 0; i < content.length(); ++i)
		if (content[i] != '\n') final_content += content[i];

	document_path = file_path;
	from_string(final_content);
}

}
