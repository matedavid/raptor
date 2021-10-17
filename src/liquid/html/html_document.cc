#include "html_document.h"

namespace liquid {

HTMLDocument::HTMLDocument()
	: head(nullptr), body(nullptr)
{
}

void HTMLDocument::add_css_block(const CSSBlock& css_block)
{
	for (SelectorValue selector :  css_block.selector_targets)
	{
		std::vector<HTMLElement*> target_elements;
		if (selector.option == SelectorValueOption::Class)
			target_elements = html->get_elements_by_class_name(selector.value);
		else if (selector.option == SelectorValueOption::ID)
			target_elements = html->get_elements_by_id(selector.value);
		else
			target_elements = html->get_elements_by_tag_name(selector.value);

		for (HTMLElement* element : target_elements)
		{
			for (auto declaration : css_block.declarations)
				element->set_style_property(declaration.first, declaration.second);
		}
	}
}

void HTMLDocument::from_string(const std::string& content)
{
	Tokenizer tokenizer = Tokenizer();
	tokenizer.tokenize(content);

	HTMLParser parser = HTMLParser();
	html = parser.parse(tokenizer);

	HTMLElement* head_element = html->get_elements_by_tag_name("head")[0];
	if (head_element != nullptr)
		head = dynamic_cast<HTMLHeadElement*>(head_element);

	HTMLElement* body_element = html->get_elements_by_tag_name("body")[0];
	if (body_element != nullptr)
		body = dynamic_cast<HTMLBodyElement*>(body_element);

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
											 (std	::istreambuf_iterator<char>()     ));

	std::string final_content = "";
	for (int i = 0; i < content.length(); ++i)
		if (content[i] != '\n') final_content += content[i];

	document_path = file_path;
	from_string(final_content);
}

}
