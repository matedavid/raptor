#pragma once

#include <iostream>

namespace liquid {

class Attribute
{
private:
	std::string m_name;
	std::string m_value;

public:
	Attribute();
	Attribute(const std::string& name, const std::string& value);

	std::string name() const;
	std::string value() const;
};

}
