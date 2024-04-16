#include "tetromino.h"
#include "console/console.h"
#include "util.h"

Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");

Tetromino::Tetromino(std::string name, int size, std::string shape) {
  name_ = name;
  size_ = size;

  for (int strIndex = 0; strIndex < shape.length(); strIndex++) {
    shape_[strIndex % size_][strIndex / size_] = shape[strIndex] == 'O';
  }
  if (name.compare("I") == 0) {
      original_ = &I;
  } else if (name.compare("O") == 0) {
      original_ = &O;
  } else if (name.compare("T") == 0) {
      original_ = &T;
  } else if (name.compare("S") == 0) {
      original_ = &S;
  } else if (name.compare("Z") == 0) {
      original_ = &Z;
  } else if (name.compare("J") == 0) {
      original_ = &J;
  } else {
      original_ = &L;
  }
}

Tetromino Tetromino::rotatedCCW() {
  int N = size();
  bool tShape[N][N];
  for (int row = 0; row < N; row++) {
    for (int col = 0; col <= row; col++) {
        tShape[row][col] = shape_[col][row];
        tShape[col][row] = shape_[row][col];
    }
  }
  for (int r = 0; r < N; r++) {
    for (int c = 0; c < N/2; c++) {
      bool temp = tShape[r][c];
      tShape[r][c] = tShape[r][N - 1 - c];
      tShape[r][N - 1 - c] = temp;
    }
  }
  std::string shapeString = "";
  for (int y = 0; y < N; y++) {
      for (int x = 0; x < N; x++) {
          shapeString.append(tShape[x][y] ? "O" : "X");
      }
  }
  return Tetromino(name(), N, shapeString);
}

Tetromino Tetromino::rotatedCW() {
  int N = size();
  bool tShape[N][N];
  for (int row = 0; row < N; row++) {
    for (int col = 0; col <= row; col++) {
      tShape[row][col] = shape_[col][row];
      tShape[col][row] = shape_[row][col];
    }
  }
  for (int c = 0; c < N; c++) {
    for (int r = 0; r < N/2; r++) {
      bool temp = tShape[r][c];
      tShape[r][c] = tShape[N - 1 - r][c];
      tShape[N - 1 - r][c] = temp;
    }
  }
  std::string shapeString = "";
  for (int y = 0; y < N; y++) {
      for (int x = 0; x < N; x++) {
          shapeString.append(tShape[x][y] ? "O" : "X");
      }
  }
  return Tetromino(name(), N, shapeString);
}

void Tetromino::drawAt(std::string s, int x, int y) {
  x++; y++;

  for (int row = 0; row < size_; row++) {
    for (int col = 0; col < size_; col++) {
      if (shape_[row][col]) {
        console::draw(x + row, y + col, s);
      }
    }
  }
}
