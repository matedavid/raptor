#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "liquid/html/html_document.h"

int main(int argc, const char* argv[])
{
	std::filesystem::path file_path = "/home/david/workspace/raptor/src/liquid/html/examples/index.html";
	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);

	std::cout << "Printing resulting HTML AST:" << std::endl;
	liquid::print_html_element(document.html, 0);

	/*
  std::string content = read_file(file_path);

	liquid::Tokenizer tokenizer = liquid::Tokenizer();
  tokenizer.tokenize(content);

	liquid::Parser html_parser = liquid::Parser();
	html_parser.parse(tokenizer);
	*/

	/*
	// ===== Tests: ===== 
	HTMLBodyElement* body = new HTMLBodyElement(nullptr);
	body->element_value = "body";

	HTMLElement* element = new HTMLElement();
	element->element_value = "ptag";
	element->set_attribute("class", "super");
	element->set_attribute("style", "no style bitch");

	HTMLElement* div = new HTMLElement();
	div->element_value = "div";

	Text* text = new Text("This is a text example.");
	text->element_value = "text";
	div->insert_child(text);

	body->insert_child_last(element);
	body->insert_child(div);
	body->print();

	HTMLElement* last_element = body->get_last_element();
	if (last_element->type() == HTMLElementType::TextType)
	{
		Text* text_element = dynamic_cast<Text*>(last_element);
		text_element->append_string(" Appended text");
		std::cout << "Is a text element: " << text_element->content() << std::endl;
	}
	// ===== ===== 
	*/


  return 0;
}
