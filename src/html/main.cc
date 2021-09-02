#include <iostream>
#include <fstream>
#include <filesystem>

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

	return 0;
}
