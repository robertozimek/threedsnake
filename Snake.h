/*
 * Snake.h - Snake Class
 * Created by Robert Ozimek
 *
 * Snake body setip
 * -----------------------------
 * (column, level, direction change)
 * -1 - no snake body at array position
 *
 * directions changes at position
 * 0 - forward
 * 1 - backward
 * 2 - up
 * 3 - left
 * 4 - down
 * 5 - right
 */

#ifndef Snake_h
#define Snake_h

#include "Arduino.h"

class Snake {
  public:
    Snake();
    void changeDirection(int direct);
    void increaseSpeed();
    int getLength();
    int getSpeed();
    bool pointOverLap(int column, int level);
    bool determineIfEats(int column, int level);
    bool shift();
    void setFood(int column, int level);
    void reset();
    int columnAt(int pos);
    int levelAt(int pos);
  private:
    int foodPoint[2];
    int snakeLength;
    int snake[10][3];
    int snakeSpeed;
    bool outOfBoundsCheck(int column, int level, int direct);
    bool incrementSnakeLength(int column, int level);
};

#endif
