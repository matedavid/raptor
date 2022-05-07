#ifndef MYCANVAS_H
#define MYCANVAS_H

#include "qsfml_canvas.h"
#include <SFML/Graphics.hpp>

class MyCanvas : public QSFMLCanvas
{
public:
  MyCanvas(QWidget *Parent, const QPoint &Position, const QSize &Size);
  void OnInit();
  void OnUpdate();

private:
  sf::Clock myClock;
  sf::Texture myImage;
  sf::Sprite mySprite;
};

#endif