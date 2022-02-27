#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "liquid/html/html_document.h"
#include "liquid/css/tokenizer.h"
#include "liquid/css/parser.h"
#include "liquid/renderer/render_tree.h"

int main_2(int argc, const char* argv[])
{
	std::filesystem::path file_path = "/home/david/workspace/raptor/examples/index.html";
	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);

	std::cout << "Printing resulting HTML AST:" << std::endl;
	//liquid::print_html_element(document.html, 0);

	//liquid::RenderBox rb(document.body, 1024, 512);
	liquid::RenderBox* box = liquid::generate_render_tree(document.body, nullptr, 1024);
	box->print(0);

  return 0;
}
