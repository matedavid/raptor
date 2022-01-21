#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "liquid/html/html_document.h"
#include "liquid/css/tokenizer.h"
#include "liquid/css/parser.h"

int main(int argc, const char* argv[])
{
	std::filesystem::path file_path = "/home/david/workspace/raptor/examples/index.html";
	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);

	std::cout << "Printing resulting HTML AST:" << std::endl;
	liquid::print_html_element(document.html, 0);

  return 0;
}