#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QThread>
#include <QMutex>
#include <iostream>
#include <fstream>
#include "engine.h"
#include "thread_imp.h"

#define KEY_C 67
#define KEY_CTRL 16777249

class ThreadImp;

namespace Ui
{
  class Widget;
}

class Widget : public QWidget
{
  Q_OBJECT

public:
  explicit Widget(QWidget *parent = 0);
  ~Widget();

  void setField(Field *);
  void setMutex(QMutex *);
  void setThreadImp(ThreadImp *);
//  void implementation(); //main cycle
  bool exit();
  bool continue_counting();
  bool *cycle_completed();
  bool *paint_completed();
  bool *do_paint();
  bool *thread_imp_completed();
  Field *field();
//  struct timespec *time(short);
  unsigned long long delay();
  void one_cycle();

protected:
  void paintEvent(QPaintEvent *);
  void closeEvent(QCloseEvent *);
  void showEvent(QShowEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);

private slots:
  void on_pushButton_iter_released();
  void on_pushButton_run_released();
  void on_pushButton_clear_released();
  void on_pushButton_random_released();
  void on_spinBox_cellsize_valueChanged(int arg1);
  void on_spinBox_fieldsize_valueChanged(int arg1);
  void on_spinBox_delay_valueChanged(int arg1);
  void on_spinBox_space_valueChanged(int arg1);

private:
  Ui::Widget *_ui;
  int _key;
  short _indent_left; //left field indent
  short _indent_top; //top field indent
  bool _do_paint; //enable to repaint
  bool _continue_counting; //enable to count
  bool _cycle_completed; //cycle completion indicator
  bool _paint_completed; //paint completion indicator
  bool _exit; //enable to quit
  bool _thread_imp_completed; //thread_imp completion indicator
  short _cell_size; //size of one cell (in pixels)
  short _interspace; //space between cells
  long long int _steps; //number of generation
//  struct timespec _time[2]; //time for delay: 0 - required, 1 - rest (optional)
  unsigned long long _delay;
  Field *_field; //pointer to field
  QMutex *_mutex; //mutex just for everybody!
  ThreadImp *_thread_imp; //thread for implementation
  QPainter _painter; //painter

  void _paint_field();
  void _one_cycle();
  void _pause();

};

#endif // WIDGET_H
