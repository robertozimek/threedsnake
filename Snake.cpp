/*
 * Snake.cpp - Snake Class Implementation
 * Created by Robert Ozimek
 */

#include "Arduino.h"
#include "Snake.h"

#define array_len(x)  (sizeof(x)/sizeof(x[0]))

// Constructor
Snake::Snake() {

  // Set start point of snake
  snake[0][0] = 27;
  snake[0][1] = 2;
  snake[0][2] = 0;

  // initialize rest of snake
  for(int i = 1; i < 10; i++) {
    snake[i][0] = -1;
    snake[i][1] = -1;
    snake[i][2] = -1;
  }

  //initial length and speed
  snakeLength = 1;
  snakeSpeed = 800;

  // Set food outside the boundries until someone sets a point
  foodPoint[0] = -2;
  foodPoint[1] = -2;
}

void Snake::changeDirection(int direct) {
  snake[0][2] = direct;
}

// Change the direction the snake is going
bool Snake::shift() {
    // Record previous level and direction of snake
    int prevLevel = snake[0][1];
    int prevDirection = snake[0][2];

    // Change movement of snake
    for (int i = 0; i < snakeLength; i++) {
      // Check if current snake if going out of bounds
      if(outOfBoundsCheck(snake[i][0], snake[i][1], snake[i][2])) {
        return false;
      }

      // Change direction of the snake
      switch (snake[i][2]) {
        case 0: // Forward
          determineIfEats(snake[i][0] + 1, snake[i][1]);
          snake[i][0] = snake[i][0] + 1;
          break;
        case 1: // Backward
          determineIfEats(snake[i][0] - 1, snake[i][1]);
          snake[i][0] = snake[i][0] - 1;
          break;
        case 2: // Up
          determineIfEats(snake[i][0], snake[i][1] + 1);
          snake[i][1] = snake[i][1] + 1;
          break;
        case 3: // Left
          determineIfEats(snake[i][0] + 8, snake[i][1]);
          snake[i][0] = snake[i][0] + 8;
          break;
        case 4: // Down
          determineIfEats(snake[i][0], snake[i][1] - 1);
          snake[i][1] = snake[i][1] - 1;
          break;
        case 5: // Right
          determineIfEats(snake[i][0] - 8, snake[i][1]);
          snake[i][0] = snake[i][0] - 8;
          break;
        default:
          break;
      }

      // Set new direction of snake and record new direction
      int temp = snake[i][2];
      snake[i][2] = prevDirection;
      prevDirection = temp;
    }
    return true;
}

// Check if snake and random choosen point for food overlap
bool Snake::pointOverLap(int column, int level) {
  for(int i = 0; i < array_len(snake); i++) {
    if(snake[i][0] == column && snake[i][1] == level) {
      return true;
    } else if((snake[i][0] == -1) || (snake[i][1] == -1)) {
      break;
    }
  }
  return false;
}

// Determine if snake reached eatable point
bool Snake::determineIfEats(int column, int level) {
  if (foodPoint[0] == column && foodPoint[1] == level) {
    incrementSnakeLength(column, level);
  }
}

void Snake::increaseSpeed() {
  snakeSpeed -= 160;
}

bool Snake::incrementSnakeLength(int column, int level) {
  // Shift snake body back
  for(int i = snakeLength - 1; i >= 0; i--) {
    snake[i+1][0] = snake[i][0];
    snake[i+1][1] = snake[i][1];
    snake[i+1][2] = snake[i][2];
  }

  snake[0][0] = column;
  snake[0][1] = level;
  snake[0][2] = snake[1][2];

  // Add size of snake length and reset random point array
  snakeLength += 1;

  increaseSpeed();

  if(snakeLength == 10) {
    return true;
  }

  return false;
}

int Snake::getLength() {
  return snakeLength;
}

int Snake::getSpeed() {
  return snakeSpeed;
}

// Check if snake is leaving the cube
bool Snake::outOfBoundsCheck(int column, int level, int direct) {
  if (((column + 1) % 8) == 0 && (column != 0) && (direct == 0 || direct == 1| direct == 3 || direct == 5)) {
    return true;
  } else if (((column) % 8) == 0 && (column != 0) && (direct == 0 || direct == 1| direct == 3 || direct == 5)) {
    return true;
  } else if ((column - 1) < 0 && (direct == 0 || direct == 1| direct == 3 || direct == 5)) {
    return true;
  } else if (((level + 1) % 4) == 0 && (direct == 2)) {
    return true;
  } else if (((level - 1) < 0) && (direct == 4)) {
    return true;
  } else if (column > 63 ) {
    return true;
  }
  return false;
}

// Sets the Location of Snake Food
void Snake::setFood(int column, int level) {
  foodPoint[0] = column;
  foodPoint[1] = level;
}

int Snake::columnAt(int pos) {
  return snake[pos][0];
}
int Snake::levelAt(int pos) {
  return snake[pos][1];
}

// Reset Snake to default
void Snake::reset() {
  snake[0][0] = 27;
  snake[0][1] = 2;
  snake[0][2] = 0;

  for(int i = 1; i < 10; i++) {
    snake[i][0] = -1;
    snake[i][1] = -1;
    snake[i][2] = -1;
    snakeSpeed = 800;
    snakeLength = 1;
  }
}
