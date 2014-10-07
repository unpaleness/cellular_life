#include "widget.h"
#include <QApplication>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <iostream>

int main(int argc, char *argv[])
{
//  std::cout << "Егорка-горка\n";
//  std::cout << "Наташка-Умиляшка\n";
  QApplication a(argc, argv);
  Widget w;
  Field f(64);
  w.setField(&f);
  QtConcurrent::run(&w, &Widget::implementation);
  w.show();
  return a.exec();
}
