#include "tetromino.h"
#include "console/console.h"
#include <cstdlib>
#include <ctime>

Tetromino generateRandomTetromino(){
  int r = rand();
  int number = r%7;
  switch (number) {
  case 0:
    return Tetromino(Tetromino::I);
    break;
  case 1:
    return Tetromino(Tetromino::O);
    break;
  case 2:
    return Tetromino(Tetromino::T);
    break;
  case 3:
    return Tetromino(Tetromino::S);
    break;
  case 4:
    return Tetromino(Tetromino::Z);
    break;
  case 5:
    return Tetromino(Tetromino::J);
    break;
  default:
    return Tetromino(Tetromino::L);
    break;
  }
}
