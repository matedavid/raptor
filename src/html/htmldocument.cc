#include "htmldocument.h"

HTMLDocument::HTMLDocument()
{
}

void HTMLDocument::load_from_file(const std::filesystem::path& file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open())
	{
		std::cout << "Error opening file: " << file_path << std::endl;
		return;
	}
	std::string content( (std::istreambuf_iterator<char>(file) ),
											 (std	::istreambuf_iterator<char>()     ));

	load_from_string(content);
}

void HTMLDocument::load_from_string(const std::string& content)
{
	Tokenizer tokenizer = Tokenizer(content);
	tokenizer.tokenize();

	Parser parser = Parser(tokenizer);
	parser.parse();

	html = parser.get_AST();
	head = html.tag(HEAD_TOKEN)[0];
	body = html.tag(BODY_TOKEN)[0];

	std::vector<Tag> titles = head.tag(TITLE_TOKEN);
	title = titles[0].text(); // Will always pick the first title
}
