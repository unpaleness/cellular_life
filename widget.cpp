#include <QMouseEvent>
#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), _ui(new Ui::Widget)
{
  _ui->setupUi(this);
  _indent_left = 110; //какая-то хрень лево
  _indent_top = 5; //какая-то хрень верх
  _do_paint = true; //do paint
  _continue_counting = false; //continue counting
  _exit = false; //exit
  _cell_size = 8; //cell size
  _steps = 0; //steps
  _time_.tv_sec = 0; //seconds
  _time_.tv_nsec = 10000000; //nanoseconds
  _time_sys.tv_sec = 1;
  _time_sys.tv_nsec = 0;
}

Widget::~Widget()
{
  delete _ui;
}

void Widget::setField(Field *field) { _field = field; }

void Widget::_paint_field()
{
  _painter.begin(this);
  QColor color;
  color = Qt::gray;
  _painter.fillRect(_indent_left, _indent_top, _field->size() * (_cell_size + 1) + 1,
                    _field->size() * (_cell_size + 1) + 1, color);
  color = Qt::green;
  for(int j = 0; j < _field->size(); j++)
    for(int i = 0; i < _field->size(); i++)
    {
      if(_field->cell(j, i) == 1)
        _painter.fillRect(_indent_left + 1 + i * (_cell_size + 1),
                          _indent_top + 1 + j * (_cell_size + 1),
                          _cell_size, _cell_size, color);
    }
  _painter.end();
}

void Widget::_one_cycle()
{
  _field->next_iteration(); //следующая итерация
  _steps++;
  _ui->lineEdit_iter->setText(QString::number(_steps));
  _do_paint = true;
  update();
}

void Widget::_pause()
{
  _continue_counting = !_continue_counting;
  if(_continue_counting)
  {
    _ui->pushButton_iter->setEnabled(false);
    _ui->pushButton_run->setText("Pause");
  }
  else
  {
    _ui->pushButton_iter->setEnabled(true);
    _ui->pushButton_run->setText("Run");
  }
}

void Widget::implementation()
{
  while(!_exit)
  {
    if(_continue_counting)
    {
      _one_cycle();
    }
    nanosleep(&_time_, &_time_2);
  }
}

/*
 * SLOTS
*/

void Widget::on_pushButton_iter_released()
{
  _one_cycle();
}

void Widget::on_pushButton_run_released()
{
  _pause();
//  nanosleep(&_time_sys, &_time_2);
  update();
}

void Widget::on_pushButton_clear_released()
{
  if(_continue_counting)
    _pause();
//  nanosleep(&_time_sys, &_time_2);
  _field->clear();
  update();
}

void Widget::on_spinBox_cellsize_valueChanged(int arg1)
{
  if(_continue_counting)
    _pause();
//  nanosleep(&_time_sys, &_time_2);
  _cell_size = arg1;
  update();
}

void Widget::on_spinBox_fieldsize_valueChanged(int arg1)
{
  if(_continue_counting)
    _pause();
//  nanosleep(&_time_sys, &_time_2);
  _field->reinitialize(arg1);
  update();
}

void Widget::on_spinBox_delay_valueChanged(int arg1)
{
  _time_.tv_nsec = arg1;
}

/*
 * EVENTS
*/

void Widget::paintEvent(QPaintEvent *)
{
  if(_do_paint)
    _paint_field();
}

void Widget::showEvent(QShowEvent *)
{
  _ui->spinBox_cellsize->setValue(_cell_size);
  _ui->spinBox_fieldsize->setValue(_field->size());
  update();
}

void Widget::moveEvent(QMoveEvent *)
{
  if(_continue_counting)
    _pause();
}

void Widget::resizeEvent(QResizeEvent *)
{
//  _ui->pushButton_run->move();
}

void Widget::mousePressEvent(QMouseEvent *)
{
  if(_continue_counting)
    _pause();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
  if(!_continue_counting)
  {
    short y = ((event->pos().y() - _indent_top) / (_cell_size + 1));
    short x = ((event->pos().x() - _indent_left) / (_cell_size + 1));
    if(y >= 0 && y < _field->size() && x >= 0 && x < _field->size())
    {
      _ui->lineEdit_x->setText(QString::number(x));
      _ui->lineEdit_y->setText(QString::number(y));
      if(_key == KEY_C)
        _field->cell(y, x) = 0;
      else
        _field->cell(y, x) = 1;
      _do_paint = true;
      update();
    }
  }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
  _key = event->key();
  _ui->lineEdit_key->setText(QString::number(_key));
}

void Widget::keyReleaseEvent(QKeyEvent *)
{
  _key = 0;
  _ui->lineEdit_key->setText(QString::number(_key));
}

void Widget::closeEvent(QCloseEvent *)
{
  _exit = true;
  QApplication::exit();
}
