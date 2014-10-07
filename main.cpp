#include "widget.h"
#include <QApplication>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Widget w;
  Field f(64);
  w.setField(&f);
  QtConcurrent::run(&w, &Widget::implementation);
  w.show();
  return a.exec();
}
