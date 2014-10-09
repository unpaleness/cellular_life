#ifndef ENGINE_H
#define ENGINE_H

#include <ctime>
#include <cstdlib>

class Field
{
public:
  Field(int size);
  ~Field();

  short &cell(int y, int x);
  int size();

  void next_iteration();
  void reinitialize(int size);
  void clear();
  void randomize(short percentage_fill);

private:
  short ***_cells;
  short _active; //which array is now active
  int _size;
  bool _has_borders; //is field limited or not

  short _live_min; //min neighbours to keep alive
  short _live_max; //max neighbours to keep alive
  short _born_min; //min neighbours to born
  short _born_max; //max neighbours to born

  void _memory_alloc();
  void _memory_delete();

  void _nullificate(short index);
  void _randomize(short percentage_fill);
  short _surroundings(int y, int x);
  short _next_cell_state(int y, int x);
  void _link_borders(); //to make field torus-like

};

#endif // ENGINE_H
