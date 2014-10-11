#include <QApplication>
//#include <QThread>
//#include <QtConcurrent/QtConcurrentRun>
#include <iostream>
#include "widget.h"
#include "thread_imp.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Widget widget;
  Field field(64);
  ThreadImp thread_imp;
  QMutex mutex(QMutex::NonRecursive);
  widget.setField(&field);
  widget.setMutex(&mutex);
  thread_imp.setWidget(&widget);
  thread_imp.setMutex(&mutex);
  widget.showMaximized();
  thread_imp.start();
  return a.exec();
}
