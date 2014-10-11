#include <QMouseEvent>
#include "widget.h"
#include "ui_widget.h"

/*
 * PUBLIC FUNCTIONS
 */

Widget::Widget(QWidget *parent) : QWidget(parent), _ui(new Ui::Widget)
{
  _ui->setupUi(this);
  _field = 0;
  _indent_left = 110; //какая-то хрень лево
  _indent_top = 5; //какая-то хрень верх
  _do_paint = true; //do paint
  _continue_counting = false; //continue counting
  _cycle_completed = true;
  _thread_imp_completed = false;
  _exit = false; //exit
  _cell_size = 8; //cell size
  _interspace = 1;
  _steps = 0; //steps
  _delay = 10000;
}

Widget::~Widget()
{
  delete _ui;
}

void Widget::setField(Field *field) { _field = field; }

void Widget::setMutex(QMutex *mutex) { _mutex = mutex; }

void Widget::setThreadImp(ThreadImp *thread_imp) { _thread_imp = thread_imp; }

bool Widget::exit() { return _exit; }

bool Widget::continue_counting() { return _continue_counting; }

bool *Widget::cycle_completed() { return &_cycle_completed; }

bool *Widget::paint_completed() { return &_paint_completed; }

bool *Widget::do_paint() { return &_do_paint; }

bool *Widget::thread_imp_completed() { return &_thread_imp_completed; }

Field *Widget::field() { return _field; }

unsigned long long Widget::delay() { return _delay; }

void Widget::one_cycle() { _one_cycle(); }

/*
 * PRIVATE FUNCTIONS
 */

void Widget::_paint_field()
{
//  _mutex->lock();
  _paint_completed = false;
  _painter.begin(this);
  QColor color;
  color = Qt::white;
  _painter.fillRect(_indent_left, _indent_top,
                    _field->size() * (_cell_size + _interspace) + _interspace,
                    _field->size() * (_cell_size + _interspace) + _interspace, color);
  color = Qt::blue;
  if(_cell_size == 1)
  {
    _painter.setPen(color);
    for(int j = 0; j < _field->size(); j++)
      for(int i = 0; i < _field->size(); i++)
      {
        if(_field->cell(j, i) == 1)
          _painter.drawPoint(_indent_left + _interspace + i * (1 + _interspace),
                             _indent_top + _interspace + j * (1 + _interspace));
      }
  }
  else
    for(int j = 0; j < _field->size(); j++)
      for(int i = 0; i < _field->size(); i++)
      {
        if(_field->cell(j, i) == 1)
          _painter.fillRect(_indent_left + _interspace + i * (_cell_size + _interspace),
                            _indent_top + _interspace + j * (_cell_size + _interspace),
                            _cell_size, _cell_size, color);
      }
  _painter.end();
  _paint_completed = true;
//  _mutex->unlock();
}

void Widget::_one_cycle()
{
  _field->next_iteration(); //следующая итерация
  _steps++;
  _ui->lineEdit_iter->setText(QString::number(_steps));
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

/*
 * SLOTS
 */

void Widget::on_pushButton_iter_released()
{
  _one_cycle();
  update();
}

void Widget::on_pushButton_run_released()
{
  _pause();
  update();
}

void Widget::on_pushButton_clear_released()
{
  if(_continue_counting)
    _pause();
  _field->clear();
  update();
}

void Widget::on_pushButton_random_released()
{
  if(_continue_counting)
    _pause();
  _steps = 0;
  _field->randomize(_ui->spinBox_random->value());
  update();
}

void Widget::on_spinBox_cellsize_valueChanged(int arg1)
{
  if(_continue_counting)
    _pause();
  _cell_size = arg1;
  update();
}

void Widget::on_spinBox_fieldsize_valueChanged(int arg1)
{
  if(_continue_counting)
    _pause();
  _field->reinitialize(arg1);
  _steps = 0;
  update();
}

void Widget::on_spinBox_delay_valueChanged(int arg1)
{
  _delay = arg1;
}

void Widget::on_spinBox_space_valueChanged(int arg1)
{
  if(_continue_counting)
    _pause();
  _interspace = arg1;
  update();
}

/*
 * EVENTS
 */

void Widget::paintEvent(QPaintEvent *)
{
  if(_do_paint && _cycle_completed)
    _paint_field();
}

void Widget::showEvent(QShowEvent *)
{
  _ui->spinBox_cellsize->setValue(_cell_size);
  _ui->spinBox_fieldsize->setValue(_field->size());
  _ui->spinBox_delay->setValue(_delay);
  update();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
  if(_continue_counting)
    _pause();
  if(!_continue_counting)
  {
    short y = ((event->pos().y() - _indent_top) / (_cell_size + _interspace));
    short x = ((event->pos().x() - _indent_left) / (_cell_size + _interspace));
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

void Widget::mouseMoveEvent(QMouseEvent *event)
{
  if(!_continue_counting)
  {
    short y = ((event->pos().y() - _indent_top) / (_cell_size + _interspace));
    short x = ((event->pos().x() - _indent_left) / (_cell_size + _interspace));
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
  while(_thread_imp->isRunning()) {}
//  _thread_imp->exit();
  QApplication::exit(0);
}

