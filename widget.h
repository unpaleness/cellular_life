#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include "engine.h"

#define KEY_C 67
#define KEY_CTRL 16777249

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

  void setField(Field *field);
  void implementation(); //main cycle

protected:
  void paintEvent(QPaintEvent *);
  void closeEvent(QCloseEvent *);
  void showEvent(QShowEvent *);
  void moveEvent(QMoveEvent *);
  void resizeEvent(QResizeEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);

private slots:
  void on_pushButton_iter_released();
  void on_pushButton_run_released();
  void on_pushButton_clear_released();
  void on_spinBox_cellsize_valueChanged(int arg1);
  void on_spinBox_fieldsize_valueChanged(int arg1);
  void on_spinBox_delay_valueChanged(int arg1);

private:
  Ui::Widget *_ui;
  int _key;
  short _do_paint; //enable to repaint
  short _indent_left; //left field indent
  short _indent_top; //top field indent
  bool _continue_counting; //eneble to count
  bool _cycle_finished; //indicates wheather main cycle finished or not
  bool _exit; //enable to quit
  short _cell_size; //size of one cell (in pixels)
  long long int _steps; //number of generation
  struct timespec //time for delay
      _time_, //required
      _time_2, //rest (optional)
      _time_sys; //delay after buttons, boxed and etc.
  Field *_field; //pointer to field
  QPainter _painter; //painter

  void _paint_field();
  void _one_cycle();
  void _pause();

};

#endif // WIDGET_H
