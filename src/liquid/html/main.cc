#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

#include "liquid/html/tokenizer.h"
#include "liquid/html/parser.h"

std::string read_file(const std::filesystem::path& file_path)
{
  std::ifstream file(file_path);
	if (!file.is_open())
	{
		std::cout << "Error opening file: " << file_path << std::endl;
    exit(0);
	}
	std::string content( (std::istreambuf_iterator<char>(file) ),
											 (std	::istreambuf_iterator<char>()     ));

  return content;
}

int main(int argc, const char* argv[])
{
  std::filesystem::path file_path = "/home/david/workspace/raptor/src/html/examples/index.html";
  std::string content = read_file(file_path);

  Tokenizer tokenizer = Tokenizer();
  tokenizer.tokenize(content);

  Parser parser = Parser();
  parser.parse(tokenizer);

  return 0;
}
