#include "game.h"
#include "tetromino.h"
#include "util.h"
#include "console/console.h"
#include <string>
#include <chrono>

void Game::drawBoard() {
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            if (board_[i][j]) {
                console::draw(i+1, j+1, BLOCK_STRING);
            }
        }
    }
}

void Game::resetBlock() {
    this->currentBlock_ = nextBlock_;
    this->nextBlock_ = generateRandomTetromino();
    currentPoint_[0] = BOARD_WIDTH / 2 - this->currentBlock_.size() / 2;
    currentPoint_[1] = 0;
    if (isCollision(currentBlock_, currentPoint_[0], currentPoint_[1])) {
        isOver_ = true;
    }
}

bool Game::stackBlock() {
    int size = this->currentBlock_.size();
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            if (this->currentBlock_.check(x, y)) {
                int boardX = currentPoint_[0] + x;
                int boardY = currentPoint_[1] + y;
                if (0 <= boardX && boardX < BOARD_WIDTH && 0 <= boardY && boardY < BOARD_HEIGHT) {
                    board_[boardX][boardY] = true;
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}

void Game::checkLine() {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int blockCount = 0;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board_[x][y]) blockCount ++;
        }
        if (blockCount == BOARD_WIDTH) {
            score_ ++;
            for (int dy = y-1; dy >= 0; dy--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    board_[x][dy+1] = board_[x][dy];
                }
            }
            for(int x = 0; x < BOARD_WIDTH; x++) {
                board_[x][0] = false;
            }
        }
        if (score_ >= LINES) {
            isOver_ = true;
        }
    }
}

void Game::drawShadow() {
    for (int y = currentPoint_[1]; y < BOARD_HEIGHT; y++) {
        if (isCollision(this->currentBlock_, currentPoint_[0], y) && y-1 >= currentPoint_[1]) {
            this->currentBlock_.drawAt(SHADOW_STRING, currentPoint_[0], y-1);
            return;
        }
    }
}

Game::Game() : currentBlock_(generateRandomTetromino()), nextBlock_(generateRandomTetromino()) {
    startTime_ = std::chrono::high_resolution_clock::now();
    isHold_ = false;
    isOver_ = false;
    holdBlock_ = nullptr;
    score_ = 0;
    moveFrame_ = DROP_DELAY;
    currentPoint_[0] = -1;
    currentPoint_[1] = -1;
    resetBlock();

    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            board_[i][j] = false;
        }
    }
}

void Game::setCurrentBlock(Tetromino block) {
    currentBlock_ = block;
}

bool Game::isCollision(Tetromino block, int x, int y) {
    int size = block.size();
    for (int dy = 0; dy < size; dy++) {
        for (int dx = 0; dx < size; dx++) {
            int checkX = x + dx, checkY = y + dy;
            bool fieldBlock = true;
            if (0 <= checkX && checkX < BOARD_WIDTH && 0 <= checkY && checkY < BOARD_HEIGHT) {
                fieldBlock = board_[checkX][checkY];
            }
            if (fieldBlock && block.check(dx, dy)) {
                return true;
            }
        }
    }
    return false;
}

std::string Game::getStringCurrentTime() {
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = currentTime - startTime_;

    long long totalMS = duration.count();
    int min = totalMS / 60000;
    int sec = (totalMS % 60000) / 1000;
    int ms = (totalMS % 1000) / 10;

    std::string timer = "";

    timer.append((min < 10 ? "0" : "") + std::to_string(min) + ":");
    timer.append((sec < 10 ? "0" : "") + std::to_string(sec) + ".");
    timer.append((ms < 10 ? "0" : "") + std::to_string(ms));

    return timer;
}
void Game::drawTimer() {
    std::string timer = getStringCurrentTime();
    int xCenter = ((BOARD_WIDTH + 2) - timer.size()) / 2;
    console::draw(xCenter >= 0 ? xCenter : 0, BOARD_HEIGHT+3, timer);
}

void Game::drawScores() {
    int sc = LINES - score_;
    std::string scoreStr = "";
    scoreStr.append(std::to_string(sc >= 0 ? sc : 0) + " ");
    scoreStr.append("lines left");
    int xCenter = ((BOARD_WIDTH + 2) - scoreStr.size()) / 2;
    console::draw(xCenter >= 0 ? xCenter : 0, BOARD_HEIGHT+2, scoreStr);
}

void Game::updateHold() {
    if (!isHold_) return;
}

void Game::drawNextBlock() {
    console::drawBox(BOARD_WIDTH + 3, 0, BOARD_WIDTH + 3 + 5, 5);
    console::draw(BOARD_WIDTH + 4, 0, "Next");

    int size = nextBlock_.size();
    nextBlock_.drawAt(BLOCK_STRING, BOARD_WIDTH + 5 - size/2, 2 - size/2);
}

void Game::drawHoldBlock() {
    console::drawBox(BOARD_WIDTH + 9, 0, BOARD_WIDTH + 9 + 5, 5);
    console::draw(BOARD_WIDTH + 10, 0, "Hold");

    if (holdBlock_ != nullptr) {
        int size = holdBlock_->size();
        holdBlock_->drawAt(BLOCK_STRING, BOARD_WIDTH + 11 - size/2, 2 - size/2);
    }
}

void Game::updateGravity() {
    moveFrame_--;
    if (moveFrame_ == 0) {
        currentPoint_[1] ++;
        moveFrame_ = DROP_DELAY;
    }
}

void Game::updateBoard() {
    if (isCollision(this->currentBlock_, currentPoint_[0], currentPoint_[1])) {
        isHold_ = false;
        currentPoint_[1] --;
        if (stackBlock()) {
            isOver_ = true;
            return;
        }
        checkLine();
        resetBlock();
    }
}

void Game::drawCurrentBlock() {
    if (!isCollision(this->currentBlock_, currentPoint_[0], currentPoint_[1])) {
        this->currentBlock_.drawAt(BLOCK_STRING, currentPoint_[0], currentPoint_[1]);
    }
}

void Game::update() {
    updateGravity();
    if (console::key(console::K_X)) {
        Tetromino tBlock = currentBlock_.rotatedCW();
        if(!isCollision(tBlock, currentPoint_[0], currentPoint_[1])) {
            this->currentBlock_ = tBlock;
        }
    } else if (console::key(console::K_Z)) {
        Tetromino tBlock = currentBlock_.rotatedCCW();
        if(!isCollision(tBlock, currentPoint_[0], currentPoint_[1])) {
            this->currentBlock_ = tBlock;
        }
    } else if (console::key(console::K_LEFT)) {
        if (!isCollision(this->currentBlock_, currentPoint_[0]-1, currentPoint_[1]))
            --currentPoint_[0];
    } else if (console::key(console::K_RIGHT)) {
        if (!isCollision(this->currentBlock_, currentPoint_[0]+1, currentPoint_[1]))
            ++currentPoint_[0];
    } else if (console::key(console::K_DOWN)) {
        ++currentPoint_[1];
    } else if (console::key(console::K_UP)) {
        for (int y = currentPoint_[1]; y < BOARD_HEIGHT; y++) {
            if (isCollision(this->currentBlock_, currentPoint_[0], y)) {
                currentPoint_[1] = y;
                break;
            }
        }
    } else if (console::key(console::K_SPACE)) {
        if (!isHold_) {
            isHold_ = true;
            if (holdBlock_ == nullptr) {
                holdBlock_ = currentBlock_.original();
                resetBlock();
            } else {
                Tetromino hold = Tetromino(*holdBlock_);
                holdBlock_ = currentBlock_.original();
                setCurrentBlock(hold);

                currentPoint_[0] = BOARD_WIDTH / 2 - this->currentBlock_.size() / 2;
                currentPoint_[1] = 0;
                if (isCollision(currentBlock_, currentPoint_[0], currentPoint_[1])) {
                    isOver_ = true;
                }
            }
        }
    } else if (console::key(console::K_ESC)) {
        exit(0);
    }
    updateBoard();
}

void Game::draw() {
    console::drawBox(0, 0, BOARD_WIDTH + 1, BOARD_HEIGHT + 1);
    drawBoard();
    drawNextBlock();
    drawHoldBlock();

    drawTimer();
    drawScores();

    if (isOver_) {
        std::string resultString = score_ >= LINES ? WIN_STRING : LOSE_STRING;
        int xCenter = ((BOARD_WIDTH + 2) - resultString.size()) / 2;
        console::draw(xCenter, BOARD_HEIGHT/2 + 1, resultString);

        if (score_ >= LINES) {
           std::string timer = getStringCurrentTime();
            xCenter = ((BOARD_WIDTH + 2) - timer.size()) / 2;
            console::draw(xCenter, BOARD_HEIGHT/2 + 2, timer);
        }
    } else {
        drawShadow();
        drawCurrentBlock();
    }
}

bool Game::shouldExit() {
    return isOver_;
}
