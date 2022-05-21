#include "render_box_marker.h"

namespace liquid {

RenderBoxMarker::RenderBoxMarker(RenderBoxMarkerType type, HTMLElement* element, float xpos, float ypos, int number)
  : marker_type(type), li_element(element), m_number(number)
{
  sf::Font font;
  if (not font.loadFromFile("../src/gui/Fonts/LiberationSans-Regular.ttf"))
    return;

  sf::Text text;
  text.setFont(font);
  text.setString(get_marker()+" ");
  text.setCharacterSize(element->style.font_size);

  float width = text.getLocalBounds().width;

  x = xpos - width;
  y = ypos;
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
