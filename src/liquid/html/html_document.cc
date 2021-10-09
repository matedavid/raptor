#include "html_document.h"

namespace liquid {

HTMLDocument::HTMLDocument()
	: head(nullptr), body(nullptr)
{
}

void HTMLDocument::from_string(const std::string& content)
{
	Tokenizer tokenizer = Tokenizer();
	tokenizer.tokenize(content);

	Parser parser = Parser();
	html = parser.parse(tokenizer);

	HTMLElement* head_element = html->get_elements_by_tag_name("head")[0];
	if (head_element != nullptr)
		head = dynamic_cast<HTMLHeadElement*>(head_element);

	HTMLElement* body_element = html->get_elements_by_tag_name("body")[0];
	if (body_element != nullptr)
		body = dynamic_cast<HTMLBodyElement*>(body_element);
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

	from_string(final_content);
}

}
