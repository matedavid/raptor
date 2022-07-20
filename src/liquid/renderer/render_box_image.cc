#include "render_box_image.h"

namespace liquid {

RenderBoxImage::RenderBoxImage(HTMLImageElement* image, RenderBox* parent)
  : image(image), RenderBox((HTMLElement*)image, parent)
{
}

AppliedDimensions RenderBoxImage::compute_dimensions(float, float)
{
  AppliedDimensions applied;

  float width_val  = image->style.width;
  float height_val = image->style.height;
  
  if (width_val != -1.f and height_val != -1.f)
  {
    content_width = width = width_val;
    content_height = height = height_val;

    return {.width=true, .height=true};
  }

  sf::Image tmp;
  if (not tmp.loadFromFile(image->src()))
  {
    std::cout << "Error loading image" << std::endl;
    exit(1);
  }

  float img_width  = (float)tmp.getSize().x;
  float img_height = (float)tmp.getSize().y;

  width = width_val != -1.f ? width_val : img_width;
  content_width = width;

  height = height_val != -1.f ? height_val : img_height;
  content_height = height;

  return {.width=true, .height=true};
}

void RenderBoxImage::print(int n_tabs)
{
  for (int i = 0; i < n_tabs; ++i)
    std::cout << "  ";

  printf("[Image pos=(%.1f, %.1f) w=%.1f h=%.1f]: %s\n", x, y, width, height, image->alt().c_str());
}

}
