#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <assert.h>

#include "tokenizer.h"
#include "token.h"

class Tag
{
public: // FOR DEBUG ONLY
	TOKEN_TYPE m_type;
	std::string m_value;
	std::vector<Tag> m_children;
	std::map<TOKEN_TYPE, std::string> m_attributes;
	bool is_closing = false;

public:
	Tag();
	Tag(TOKEN_TYPE type, bool is_closing);
	Tag(TOKEN_TYPE type, const std::string& value);

	void set_value(const std::string& value);
	void add_child(Tag child);
	void add_atribute(TOKEN_TYPE attr_type, const std::string& attr_value);

	std::vector<Tag> innerHTML() const;
	std::string text() const;

	std::pair<bool, std::string> attribute(TOKEN_TYPE attr_type) const;
};

class Parser
{
private:
	Tokenizer& m_tokenizer;
	std::vector<Tag> m_tags;

private:
	static Tag parse_tag(Tokenizer& tokenizer);
	void convert_into_tags();

public:
	Parser(Tokenizer& tokeznizer);

	void parse();
};
