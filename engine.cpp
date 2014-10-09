#include "engine.h"

/*
 * PUBLIC FUCNTIONS
*/

Field::Field(int size)
{
  srand(time(0));
  _size = size;
  _memory_alloc();
  _active = 0;
  _has_borders = false;
  clear();
  _live_min = 2;
  _live_max = 3;
  _born_min = 3;
  _born_max = 3;
}

Field::~Field()
{
  _memory_delete();
}

short &Field::cell(int y, int x) { return _cells[_active][y + 1][x + 1]; }

int Field::size() { return _size; }

void Field::reinitialize(int size)
{
  _memory_delete();
  _size = size;
  _memory_alloc();
  clear();
}

void Field::clear()
{
  _nullificate(0);
  _nullificate(1);
}

void Field::randomize(short percentage_fill) { _randomize(percentage_fill); }

void Field::next_iteration()
{
  for(int j = 1; j < _size + 1; j++)
    for(int i = 1; i < _size + 1; i++)
      _cells[1 - _active][j][i] = _next_cell_state(j, i);
  _active = 1 - _active;
  _link_borders();
}

/*
 * PRIVATE FUNCTIONS
*/

void Field::_memory_alloc()
{
  _cells = new short **[2];
  for(int j = 0; j < 2; j++)
  {
    _cells[j] = new short *[_size + 2];
    for(int i = 0; i < _size + 2; i++)
      _cells[j][i] = new short [_size + 2];
  }
}

void Field::_memory_delete()
{
  for(int j = 0; j < 2; j++)
  {
    for(int i = 0; i < _size + 2; i++)
      delete [] _cells[j][i];
    delete [] _cells[j];
  }
  delete [] _cells;
}

short Field::_next_cell_state(int y, int x)
{
  short surr = _surroundings(y, x);
  switch(_cells[_active][y][x])
  {
    case 0: //cell is dead
      if(surr >= _born_min && surr <= _born_max)
        return 1;
      else
        return 0;
      break;
    case 1: //cell is alive!
      if(surr >= _live_min && surr <= _live_max)
        return 1;
      else
        return 0;
      break;
    default:
      return 0;
      break;
  }
}

short Field::_surroundings(int y, int x)
{
  return _cells[_active][y - 1][x - 1] + _cells[_active][y - 1][x] +
      _cells[_active][y - 1][x + 1] + _cells[_active][y][x - 1] +
      _cells[_active][y][x + 1] + _cells[_active][y + 1][x - 1] +
      _cells[_active][y + 1][x] + _cells[_active][y + 1][x + 1];
}

void Field::_nullificate(short index)
{
  for(int j = 0; j < _size + 2; j++)
    for(int i = 0; i < _size + 2; i++)
    {
      _cells[index][j][i] = 0;
    }
}

void Field::_randomize(short percentage_fill)
{
  for(int j = 1; j < _size + 1; j++)
    for(int i = 1; i < _size + 1; i++)
      if(rand() % 100 < percentage_fill) _cells[_active][j][i] = 1;
      else _cells[_active][j][i] = 0;
}

//in case of torus this function copies border values to the opposite sides
void Field::_link_borders()
{
  for(int n = 1; n < _size + 1; n++)
  {
    _cells[_active][n][0] = _cells[_active][n][_size];
    _cells[_active][n][_size + 1] = _cells[_active][n][1];
    _cells[_active][0][n] = _cells[_active][_size][n];
    _cells[_active][_size + 1][n] = _cells[_active][1][n];
  }
  _cells[_active][0][0] = _cells[_active][_size][_size];
  _cells[_active][0][_size + 1] = _cells[_active][_size][1];
  _cells[_active][_size + 1][0] = _cells[_active][1][_size];
  _cells[_active][_size + 1][_size + 1] = _cells[_active][1][1];
}
