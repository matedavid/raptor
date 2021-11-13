#include "html_ordered_list_element.h"

namespace liquid {

HTMLOrderedListElement::HTMLOrderedListElement()
{
}

std::vector<HTMLElement*> HTMLOrderedListElement::get_list_elements()
{
  return get_elements_by_tag_name("li");
}

}