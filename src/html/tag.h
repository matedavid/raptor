#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <utility>

#include "token.h"

class Tag
{
private:
	TOKEN_TYPE m_type;
	std::string m_value;
	std::vector<Tag> m_children;
	std::map<TOKEN_TYPE, std::string> m_attributes;
	bool m_closing = false;

public:
	Tag();
	Tag(TOKEN_TYPE type, bool is_closing);
	Tag(TOKEN_TYPE type, const std::string& value, bool is_closing);

	void set_value(const std::string& value);
	void add_child(Tag child);
	void add_atribute(TOKEN_TYPE attr_type, const std::string& attr_value);

	std::vector<Tag> innerHTML() const;
	std::string text() const;

	TOKEN_TYPE type() const;
	bool is_closing() const;
	std::pair<bool, std::string> attribute(TOKEN_TYPE attr_type) const;
};
