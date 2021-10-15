#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "liquid/html/html_document.h"
#include "liquid/css/tokenizer.h"

std::string read_file(const std::filesystem::path& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::cout << "Error opening file: " << path << std::endl;
		exit(0);
	}
	std::string content( (std::istreambuf_iterator<char>(file) ),
											 (std	::istreambuf_iterator<char>()     ));

	std::string final_content = "";
	for (int i = 0; i < content.length(); ++i)
		if (content[i] != '\n') final_content += content[i];

	return final_content;
}

void css_tests()
{
	std::filesystem::path file_path = "/home/david/workspace/raptor/src/liquid/html/examples/style.css";
	std::string content = read_file(file_path);

	liquid::CSSTokenizer tokenizer = liquid::CSSTokenizer();
	tokenizer.tokenize(content);
}

int main(int argc, const char* argv[])
{
	/*
	std::filesystem::path file_path = "/home/david/workspace/raptor/src/liquid/html/examples/index.html";
	liquid::HTMLDocument document = liquid::HTMLDocument();
	document.from_file(file_path);

	std::cout << "Printing resulting HTML AST:" << std::endl;
	liquid::print_html_element(document.html, 0);
	*/

	css_tests();

  return 0;
}
