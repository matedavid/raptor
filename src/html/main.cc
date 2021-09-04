#include <iostream>
#include <fstream>
#include <filesystem>

#include "htmldocument.h"

void print_tags_recursive(const Tag& tag, int spaces)
{
	for (int i = 0; i < spaces; ++i)
		std::cout << "\t";

	if (tag.type() == TEXT_TOKEN)
		std::cout << tag << std::endl;
	else
	{
		std::cout << tag << std::endl;
		std::vector<Tag> children = tag.innerHTML();
		for (int i = 0; i < children.size(); ++i)
			print_tags_recursive(children[i], spaces+1);
	}
}

/*
int main()
{
	std::filesystem::path path = std::filesystem::path("/home/david/workspace/raptor/src/html/examples/index.html");

	HTMLDocument html = HTMLDocument();
	html.load_from_file(path);

	std::cout << "Title: " << html.title << std::endl;

	std::cout << "Printing Head:" << std::endl;
	print_tags_recursive(html.head, 0);

	std::cout << "Printing Body:" << std::endl;
	print_tags_recursive(html.body, 0);

	return 0;
}
*/
