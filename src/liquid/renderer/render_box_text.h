#pragma once

#include "liquid/renderer/render_box.h"
#include "liquid/html/text.h"

namespace liquid {

class RenderBoxText : public RenderBox
{
private:
  Text* text;

public:
  RenderBoxText(Text* text, RenderBox* parent);

  std::string content() const;

  void print(int n_tabs) override;
};

}
