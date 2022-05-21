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
  HTMLElement* li_element;
  RenderBoxMarkerType marker_type;
  int m_number;

public:
  RenderBoxMarker(RenderBoxMarkerType type, HTMLElement* element, float xpos, float ypos);

  RenderBoxType type() const override { return RenderBoxType::Marker; }
  bool is_printable() const override { return true; }

  void set_number(int number);

  std::string get_marker() const;
  float get_font_size() const;
  Color get_text_color() const;
};

}
