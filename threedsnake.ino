/*
 *  3D Snake LED CUBE
 *  
 *  HARDWARE:
 *  74HC595 Shift Registers - led column control
 *  MOSFET - led layer control
 *  6 buttons - control the snake
 *  
 *  SOURCES
 *  ShiftPWM Library by Elco Jacobs http://www.elcojacobs.com/shiftpwm/
 *  
 */

#define array_len(x)  (sizeof(x)/sizeof(x[0]))

// Pin 8 for shift register latch
// Data & Clock pin are define in the shiftpwn library using the SPI pins
const int ShiftPWM_latchPin=8;

// Inverts the output of shift register so LOW is HIGH and HIGH is LOW
const bool ShiftPWM_invertOutputs = true;

// Turn off option to shift by 8 at a time compare to all at once
const bool ShiftPWM_balanceLoad = false;

// ShiftPWM Library by Elco Jacobs
// http://www.elcojacobs.com/shiftpwm/
// https://github.com/elcojacobs/ShiftPWM/
#include <ShiftPWM.h> 

// Set up options for the ShiftPWM library
unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
unsigned int numRegisters = 8;
unsigned int numOutputs = numRegisters*8;
unsigned int numRGBLeds = numRegisters*8/3;
unsigned int fadingMode = 0; 
unsigned long startTime = 0;


boolean gameStarted = false; // Boolean used to start game when button is pressed

// Controller Pins
int upPin = 20;
int rightPin = 3;
int downPin = 21;
int leftPin = 19;
int forwardPin = 2;
int backPin = 18;

// LED cube layer pins
int layerPins[4] = {4, 5, 6, 7};

// Snake body array
// -----------------------------
// (column, level, direction change)
// -1 - no snake body at array position
//
// directions changes at position
// 0 - forward
// 1 - backward
// 2 - up
// 3 - left
// 4 - down
// 5 - right
int snake[10][3] = {
  { 27,  2,  0},  // (column, level in array, direction change) 
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1},
  {  -1,  -1, -1}
};

int snakeLength = 1;

// Random point for snake to eat
int randomPoint[2] = {-1, -1}; // column, level

// Snake speed
int snakeSpeed = 800; // inital speed

void setup() {
  pinMode(7,OUTPUT);
  // Loops over layer pins and sets to output
  for (int i = 0; i < 4; i++) {
    pinMode(layerPins[i], OUTPUT);
  }
  
  // Seed random number using noise from analog pin 0
  randomSeed(analogRead(0));

  // Set up controller pins
  pinMode(upPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(forwardPin, INPUT);
  pinMode(backPin, INPUT);

  // Hardware interrupt for buttons
  attachInterrupt(digitalPinToInterrupt(upPin), upPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(rightPin), rightPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(downPin), downPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(leftPin), leftPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(forwardPin), forwardPressed, RISING);
  attachInterrupt(digitalPinToInterrupt(backPin), backPressed, RISING);

  // Set number of registers used
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // Start the shift PWM 
  ShiftPWM.Start(pwmFrequency,maxBrightness);
}

/*
 * Button Press Functions
 */
void startGameIfNotStarted() {
  if(!gameStarted) {
    gameStarted = true;
  }
}

void upPressed() {
  startGameIfNotStarted();
  snake[0][2] = 2;
}

void rightPressed() {
  startGameIfNotStarted();
  snake[0][2] = 5;
}

void downPressed() {
  startGameIfNotStarted();
  snake[0][2] = 4;
}

void leftPressed() {
  if(!gameStarted) {
    gameStarted = true;
  }
  snake[0][2] = 3;
}

void forwardPressed() {
  startGameIfNotStarted();
  snake[0][2] = 0;
}

void backPressed() {
  startGameIfNotStarted();
  snake[0][2] = 1;
}


// Print score of game
void gameOver(int score = 0) {
  for(int i = 0; i < 3; i++) {
    digitalWrite(layerPins[i], LOW);
  }
  digitalWrite(layerPins[3], HIGH);
  
  switch(score) {
    case 1:
      ShiftPWM.SetOne(46,maxBrightness);
      for(int i = 40; i >= 32; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(24,maxBrightness);
      break;
    case 2:
      ShiftPWM.SetOne(47,maxBrightness);
      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      for(int i = 23; i >= 20; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(28,maxBrightness);
      ShiftPWM.SetOne(36,maxBrightness);
      for(int i = 44; i >= 40; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(32,maxBrightness);
      ShiftPWM.SetOne(24,maxBrightness);
      ShiftPWM.SetOne(16,maxBrightness);
      break;
    case 3:
      ShiftPWM.SetOne(47,maxBrightness);
      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      for(int i = 23; i >= 16; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(28,maxBrightness);
      ShiftPWM.SetOne(36,maxBrightness);
      ShiftPWM.SetOne(44,maxBrightness);
      ShiftPWM.SetOne(40,maxBrightness);
      ShiftPWM.SetOne(32,maxBrightness);
      ShiftPWM.SetOne(24,maxBrightness);
      break;
    case 4:
      for(int i = 55; i >= 52; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(44,maxBrightness);
      ShiftPWM.SetOne(36,maxBrightness);
      ShiftPWM.SetOne(28,maxBrightness);
      ShiftPWM.SetOne(20,maxBrightness);
       for(int i = 15; i >= 8; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      break;
    case 5:
      ShiftPWM.SetOne(15,maxBrightness);
      ShiftPWM.SetOne(23,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(47,maxBrightness);
      for(int i = 55; i >= 52; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(44,maxBrightness);
      ShiftPWM.SetOne(36,maxBrightness);
      ShiftPWM.SetOne(28,maxBrightness);
      ShiftPWM.SetOne(20,maxBrightness);
      for(int i = 12; i >= 8; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(16,maxBrightness);
      ShiftPWM.SetOne(24,maxBrightness);
      ShiftPWM.SetOne(32,maxBrightness);
      ShiftPWM.SetOne(40,maxBrightness);
      ShiftPWM.SetOne(48,maxBrightness);
      break;
    case 6:
      ShiftPWM.SetOne(47,maxBrightness);
      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      ShiftPWM.SetOne(23,maxBrightness);
      ShiftPWM.SetOne(15,maxBrightness);
      for(int i = 55; i >= 48; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(44,maxBrightness);
      ShiftPWM.SetOne(36,maxBrightness);
      ShiftPWM.SetOne(28,maxBrightness);
      ShiftPWM.SetOne(20,maxBrightness);
      for(int i = 12; i >= 8; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(16,maxBrightness);
      ShiftPWM.SetOne(24,maxBrightness);
      ShiftPWM.SetOne(32,maxBrightness);
      ShiftPWM.SetOne(40,maxBrightness);
      break;
    case 7:
      ShiftPWM.SetOne(55,maxBrightness);
      ShiftPWM.SetOne(47,maxBrightness);
      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      ShiftPWM.SetOne(23,maxBrightness);
      for(int i = 15; i >= 8; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      break;
    case 8:
      for(int i = 55; i >= 48; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(47,maxBrightness);
      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      ShiftPWM.SetOne(23,maxBrightness);
      ShiftPWM.SetOne(44,maxBrightness);
      ShiftPWM.SetOne(36,maxBrightness);
      ShiftPWM.SetOne(28,maxBrightness);
      ShiftPWM.SetOne(20,maxBrightness);
      for(int i = 15; i >= 8; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(40,maxBrightness);
      ShiftPWM.SetOne(32,maxBrightness);
      ShiftPWM.SetOne(24,maxBrightness);
      ShiftPWM.SetOne(16,maxBrightness);
      break;
    case 9:
      for(int i = 55; i >= 52; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(47,maxBrightness);
      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      ShiftPWM.SetOne(23,maxBrightness);
      ShiftPWM.SetOne(44,maxBrightness);
      ShiftPWM.SetOne(36,maxBrightness);
      ShiftPWM.SetOne(28,maxBrightness);
      ShiftPWM.SetOne(20,maxBrightness);
      for(int i = 15; i >= 8; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      break;
    case 10:
      for(int i = 63; i >= 56; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      for(int i = 47; i >= 40; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }

      ShiftPWM.SetOne(39,maxBrightness);
      ShiftPWM.SetOne(31,maxBrightness);
      ShiftPWM.SetOne(23,maxBrightness);
      ShiftPWM.SetOne(15,maxBrightness);
      for(int i = 7; i >= 0; i--) {
         ShiftPWM.SetOne(i,maxBrightness);
      }
      ShiftPWM.SetOne(32,maxBrightness);
      ShiftPWM.SetOne(24,maxBrightness);
      ShiftPWM.SetOne(16,maxBrightness);
      ShiftPWM.SetOne(8,maxBrightness);
      break;
    default:
      break;
  }

  resetGameSettings();
  delay(5000);
  digitalWrite(layerPins[3], LOW);
}


// Reset game to start from the beginning
void resetGameSettings() {
  snake[0][0] = 27;
  snake[0][1] = 2;
  snake[0][2] = 0;

  for(int i = 1; i < 10; i++) {
    snake[i][0] = -1;
    snake[i][1] = -1;
    snake[i][2] = -1;
  }
  
  snakeSpeed = 800;
  snakeLength = 1;
  randomPoint[0] = -1;
  randomPoint[1] = -1;
  gameStarted = false;
}


// Get random point for snake to eat
void getRandomPoint() {
  randomPoint[0] = random(0, 63);
  randomPoint[1] = random(0,3);

  // Make sure the random point is not overlapping the snake
  for(int i = 0; i < array_len(snake); i++) {
    if(snake[i][0] == randomPoint[0] && snake[i][1] == randomPoint[1]) {
      randomPoint[0] = random(0, 63);
      randomPoint[1] = random(0,3);
      i = 0;
    } else if((snake[i][0] == -1) || (snake[i][1] == -1)) {
      break;
    } 
  }
}


// Check if snake goes out of bound
void outOfBoundsCheck(int column, int level, int direct) {
  if (((column + 1) % 8) == 0 && (column != 0) && (direct == 0 || direct == 1| direct == 3 || direct == 5)) {
    gameOver(snakeLength);
  } else if (((column) % 8) == 0 && (column != 0) && (direct == 0 || direct == 1| direct == 3 || direct == 5)) {
    gameOver(snakeLength);
  } else if ((column - 1) < 0 && (direct == 0 || direct == 1| direct == 3 || direct == 5)) {
    gameOver(snakeLength);
  } else if (((level + 1) % 4) == 0 && (direct == 2)) {
    gameOver(snakeLength);
  } else if (((level - 1) < 0) && (direct == 4)) {
    gameOver(snakeLength);
  } else if (column > 63 ) {
    gameOver(snakeLength);
  }
}

void checkIfEats(int column, int level) {
  if(randomPoint[0] == column && randomPoint[1] == level) {
    // Shift snake body back
    for(int i = snakeLength - 1; i >= 0; i--) {
      snake[i+1][0] = snake[i][0];
      snake[i+1][1] = snake[i][1];
      snake[i+1][2] = snake[i][2]; 
    }
    
    // Add a piece of snake to the front
    snake[0][0] = column;
    snake[0][1] = level;
    snake[0][2] = snake[1][2];

    // Add size of snake length and reset random point array
    snakeLength += 1;
    randomPoint[0] = -1;
    randomPoint[1] = -1;

    // Increase game speed
    snakeSpeed -= 200;

    // Check if snake is 100% full, if so display score and restart game
    if(snakeLength == 10) {
      gameOver(10);
    }
  }
}


// boolean to check if the arduino was just turned
int firstRun = true;


void loop() {

  // Cool startup animationg, turning on each layer 1 by 1 and speeding up
  int delayTime = 300;
  while(firstRun) {
    for(int i = 0; i < 10; i++) {
      for(int x = 0; x < 4; x++) {
            digitalWrite(layerPins[x], HIGH);
            ShiftPWM.SetAll(maxBrightness);
            delay(delayTime);
            ShiftPWM.SetAll(0);
            digitalWrite(layerPins[x], LOW);
            delayTime -= 5;
      }
    }
    firstRun = false;
  }
  

  // Start game once button is pressed
  while(gameStarted) {
    // Retrieve a random point for snake to eat
    if(randomPoint[0] == -1 || randomPoint[1] == -1) {
      getRandomPoint();
    }

    // Record previous level and direction of snake
    int prevLevel = snake[0][1];
    int prevDirection = snake[0][2];

    // Change movement of snake
    for(int i = 0; i < snakeLength; i++) {
      // Check if current snake if going out of bounds
      outOfBoundsCheck(snake[i][0],snake[i][1], snake[i][2]);

      
      // Change direction of the snake 
      switch(snake[i][2]) {
        case 0: // Forward
          checkIfEats(snake[i][0] + 1, snake[i][1]);
          snake[i][0] = snake[i][0] + 1;
          break;
        case 1: // Backward
          checkIfEats(snake[i][0] - 1, snake[i][1]);
          snake[i][0] = snake[i][0] - 1;
          break;
        case 2: // Up
          checkIfEats(snake[i][0], snake[i][1] + 1);
          snake[i][1] = snake[i][1] + 1;
          break;
        case 3: // Left
          checkIfEats(snake[i][0] + 8, snake[i][1]);
          snake[i][0] = snake[i][0] + 8;
          break;
        case 4: // Down
          checkIfEats(snake[i][0], snake[i][1] - 1);
          snake[i][1] = snake[i][1] - 1;
          break;
        case 5: // Right
          checkIfEats(snake[i][0] - 8, snake[i][1]);
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

    // Loop through so that LEDs are on for certain amount of time according to snake speed
    for(int j = 0; j < snakeSpeed; j++) {
      // Loop through cube layers
      for(int x = 0; x < 4; x++) {
        digitalWrite(layerPins[x], HIGH);
        // Loop through snake and light LEDs 
        for(int i = 0; i < snakeLength; i++) {
            if(randomPoint[1] == x) {
              ShiftPWM.SetOne(randomPoint[0], maxBrightness); // Light random snake food
            }
            if(snake[i][1] == x) {
              ShiftPWM.SetOne(snake[i][0], maxBrightness); // Light Snake
            }
        }
        ShiftPWM.SetAll(0); // Turn off shift registers
        digitalWrite(layerPins[x], LOW); // Turn off layer
      }
    }
  }
}
