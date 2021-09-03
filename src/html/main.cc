#include <iostream>
#include <fstream>
#include <filesystem>

#include "tokenizer.h"
#include "parser.h"

std::string read_file(std::filesystem::path& file_path)
{
	std::ifstream file(file_path);
	if (!file.is_open())
	{
		std::cout << "Error opening file!" << std::endl;
		return "";
	}
	std::string content( (std::istreambuf_iterator<char>(file) ),
											 (std::istreambuf_iterator<char>()     ));

	return content;
}

int main()
{
	std::filesystem::path file_path("/home/david/workspace/raptor/src/html/examples/index.html");
	std::string content = read_file(file_path);

	Tokenizer tokenizer = Tokenizer();
	tokenizer.tokenize(content);

	/*
	while (tokenizer.current().type != EOF_TOKEN)
	{
		const Token token = tokenizer.current();
		std::cout << "(" << type_as_string(token.type) << ", " << token.value << ")" << std::endl;
		tokenizer.next();
	}
	*/

	Parser parser = Parser(tokenizer);
	parser.parse();

	return 0;
}
