#include "html_unordered_list_element.h"

namespace liquid {

HTMLUnorderedListElement::HTMLUnorderedListElement()
{
}

std::vector<HTMLElement*> HTMLUnorderedListElement::get_list_elements()
{
  return get_elements_by_tag_name("li");
}

}