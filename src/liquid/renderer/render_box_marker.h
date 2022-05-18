#pragma once

#include "render_box.h"

namespace liquid {

enum RenderBoxMarkerType 
{
  OrderedList,
  UnorderedList
};

class RenderBoxMarker : public RenderBox 
{
private:
  RenderBoxMarkerType marker_type;

public:
  RenderBoxMarker(RenderBoxMarkerType type, float xpos, float ypos);
  RenderBoxType type() const override { return RenderBoxType::Marker; }

  std::string get_marker() const;
};

}
