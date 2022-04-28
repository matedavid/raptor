#include <iostream>

#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  QWidget window;
  window.resize(1024, 960);
  window.setWindowTitle("Example title");
  window.show();

  return app.exec();
}

