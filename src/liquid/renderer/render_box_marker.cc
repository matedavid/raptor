#include "render_box_marker.h"

namespace liquid {

RenderBoxMarker::RenderBoxMarker(RenderBoxMarkerType type, HTMLElement* element, float xpos, float ypos)
  : marker_type(type), li_element(element)
{
  x = xpos;
  y = ypos;
}

void RenderBoxMarker::set_number(int number)
{
  m_number = number;
}

std::string RenderBoxMarker::get_marker() const 
{
  if (marker_type == RenderBoxMarkerType::OrderedList)
    return std::to_string(m_number) + ".";
  else if (marker_type == RenderBoxMarkerType::UnorderedList)
    return "*";

  return "";
}

float RenderBoxMarker::get_font_size() const
{
  return li_element->style.font_size;
}

Color RenderBoxMarker::get_text_color() const 
{
  return li_element->style.color;
}

}
