#ifndef THREAD_IMP_H
#define THREAD_IMP_H

#include <QThread>
#include <QMutex>
#include "widget.h"

class Widget;

class ThreadImp : public QThread
{
public:
  ThreadImp();
  void setWidget(Widget *);
  void setMutex(QMutex *);
  void run();

 private:
  Widget *_w;
  QMutex *_m;

};

#endif // THREAD_IMP_H
