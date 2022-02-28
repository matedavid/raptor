#include "render_box_img.h"

namespace liquid {

RenderBoxImage::RenderBoxImage()
{
}

RenderBoxImage::RenderBoxImage(HTMLImageElement* img_element, RenderBox* parent)
{
  this->parent = parent;
  node = img_element;

  // Compute x, y position
  x = parent->get_x();
  y = parent->get_y();

  // Compute width and height
  img_width = img_element->style.width;
  img_height = img_element->style.height;
}

std::string RenderBoxImage::get_src() const
{
  HTMLImageElement* img_element = dynamic_cast<HTMLImageElement*>(node);
  if (img_element == nullptr)
    return "";

  return img_element->src();
}

std::string RenderBoxImage::get_alt() const
{
  HTMLImageElement* img_element = dynamic_cast<HTMLImageElement*>(node);
  if (img_element == nullptr)
    return "";

  return img_element->alt();
}

}