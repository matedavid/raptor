#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include "tokenizer.h"
#include "parser.h"
#include "tag.h"

class HTMLDocument
{
public:
	Tag html;
	Tag head;
	Tag body;

	std::string title;

public:
	HTMLDocument();

	void load_from_file(const std::filesystem::path& file_path);
	void load_from_string(const std::string& content);
};
