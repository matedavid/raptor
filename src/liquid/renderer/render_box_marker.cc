#include "render_box_marker.h"

namespace liquid {

RenderBoxMarker::RenderBoxMarker(RenderBoxMarkerType type, float xpos, float ypos)
  : marker_type(type)
{
  x = xpos;
  y = ypos;
}

std::string RenderBoxMarker::get_marker() const 
{
  if (marker_type == RenderBoxMarkerType::OrderedList)
    return "1.";
  else if (marker_type == RenderBoxMarkerType::UnorderedList)
    return "·";

  return "";
}

}
