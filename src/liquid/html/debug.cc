#include "debug.h"

namespace liquid {

void print_html_element(HTMLElement* element, int tab_num)
{
	if (element == nullptr)
		return;

	for (int t = 0; t < tab_num; ++t) std::cout << "  ";

  std::map<std::string, Attribute*> attributes = element->get_attributes();

	// Print information of element
	std::cout << "[" << element->element_value;

	auto attr_it = attributes.begin();
	while (attr_it != attributes.end())
	{
		Attribute* attr = attr_it->second;
		std::cout << " " << attr->name() << ":" << attr->value();
		++attr_it;
	}

  if (element->type() == HTMLElementType::TextType)
  {
    Text* text_element = dynamic_cast<Text*>(element);
    std::cout << " Content=" << text_element->content();
  }

	std::cout << "]" << std::endl;
  
  std::vector<HTMLElement*> children = element->child_elements();
	for (int i = 0; i < children.size(); ++i)
		print_html_element(children[i], tab_num+1);
}

}
