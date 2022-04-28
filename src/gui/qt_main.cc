#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QMainWindow window;

  auto* widget = new QWidget(&window);
  auto* layout = new QVBoxLayout(widget);

  window.setCentralWidget(widget);
  widget->setLayout(layout);

  layout->addWidget(new QPushButton("Hello world", widget));
  layout->addWidget(new QPushButton("Hello world 2", widget));

  window.show();
  return app.exec();
}

