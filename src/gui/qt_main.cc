/*
#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QMainWindow window;

  auto *widget = new QWidget(&window);
  auto *layout = new QVBoxLayout(widget);

  window.setCentralWidget(widget);
  widget->setLayout(layout);

  auto *search_bar = new QTextEdit();
  layout->addWidget(search_bar);

  window.show();
  return app.exec();
}
*/

#include <QApplication>
#include "my_canvas.h"
#include <QFrame>

int main(int argc, char *argv[])
{
  QApplication App(argc, argv);
  // Create the main frame
  QFrame *MainFrame = new QFrame;
  MainFrame->setWindowTitle("Qt SFML");
  MainFrame->resize(400, 400);
  MainFrame->show();
  // Create a SFML view inside the main frame
  MyCanvas *SFMLView = new MyCanvas(MainFrame, QPoint(20, 20), QSize(360, 360));
  SFMLView->show();
  return App.exec();
}
