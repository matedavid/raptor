#include <iostream>
#include <fstream>
#include <filesystem>

#include "tokenizer.h"

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

	for (int i = 0; i < tokenizer.m_tokens.size(); ++i)
	{
		Token token = tokenizer.m_tokens[i];
		std::cout << token.type_as_string() << " " << token.value();
		if (token.type() != TEXT_TOKEN)
			std::cout << " " << "Is closing: " << token.is_closing_tag();
		std::cout << std::endl;
	}

	return 0;
}
