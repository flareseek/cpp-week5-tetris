
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"
#include <chrono>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 5

#define DROP_DELAY 60

#define WIN_STRING "You Win"
#define LOSE_STRING "You Lose"

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 고정된 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];

  Tetromino currentBlock_;

  Tetromino nextBlock_;

  Tetromino* holdBlock_;

  bool isHold_;

  bool isOver_;

  std::chrono::high_resolution_clock::time_point startTime_;

  int score_;

  int moveFrame_;

  int currentPoint_[2];

  void setCurrentBlock(Tetromino block);

  bool isCollision(Tetromino block, int x, int y);

  void resetBlock();

  // border에 닿았는지
  bool stackBlock();

  void drawBoard();

  void drawShadow();

  void drawNextBlock();

  void drawHoldBlock();

  void drawCurrentBlock();

  void checkLine();

  void drawTimer();

  void drawScores();

  void updateHold();

  void updateGravity();

  void updateBoard();

  std::string getStringCurrentTime();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif
