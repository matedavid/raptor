#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "liquid/html/html_document.h"
#include "liquid/css/tokenizer.h"
#include "liquid/css/parser.h"

/*
int main(int argc, const char* argv[])
{
	std::filesystem::path file_path = "/home/david/workspace/raptor/examples/index.html";
	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);

	std::cout << "Printing resulting HTML AST:" << std::endl;
	liquid::print_html_element(document.html, 0);

	auto elements = document.html->get_elements_by_class_name("example-text");
	for (auto elem : elements)
	{
		std::cout << "element 1: " << std::endl;
		std::cout << "------" << std::endl;
		print_style(elem->style);
	}

  return 0;
}
*/
