#include "thread_imp.h"

ThreadImp::ThreadImp() { _w = 0; }

void ThreadImp::setWidget(Widget *w) { _w = w; }

void ThreadImp::setMutex(QMutex *m) { _m = m; }

void ThreadImp::run()
{
  while(!_w->exit())
  {
      if(_w->continue_counting() && _w->paint_completed())
      {
        *_w->cycle_completed() = false;
        _w->one_cycle();
        *_w->do_paint() = true;
        *_w->cycle_completed() = true;
        _w->update();
      }
      usleep(_w->delay());
  }
  *_w->thread_imp_completed() = true;
}
