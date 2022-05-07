#include "my_canvas.h"

MyCanvas::MyCanvas(QWidget *Parent, const QPoint &Position, const QSize &Size) : QSFMLCanvas(Parent, Position, Size)
{
}

void MyCanvas::OnInit()
{
  RenderWindow::clear(sf::Color::White);

  sf::Font font;
  if (not font.loadFromFile("../src/gui/Fonts/LiberationSans-Regular.ttf"))
    return;

  sf::Text text;
  text.setFont(font);
  text.setString("Hello world");

  text.setPosition(0, 0);
  text.setCharacterSize(20);

  text.setFillColor(sf::Color::Black);

  RenderWindow::draw(text);

  myClock.restart();
}

void MyCanvas::OnUpdate()
{
  //RenderWindow::clear(sf::Color(0, 128, 0));
  //myClock.restart();
}
