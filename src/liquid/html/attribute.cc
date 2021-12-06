#include "attribute.h"

namespace liquid
{

Attribute::Attribute()
{
}

Attribute::Attribute(const std::string& name, const std::string& value)
	: m_name(name), m_value(value)
{
}

void Attribute::set_name(const std::string& name)
{
	m_name = name;
}

void Attribute::set_value(const std::string& value)
{
	m_value = value;
}

std::string Attribute::name() const
{
	return m_name;
}

std::string Attribute::value() const
{
	return m_value;
}

}
