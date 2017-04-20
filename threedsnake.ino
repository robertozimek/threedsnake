/*
    3D Snake LED CUBE

    HARDWARE:
    74HC595 Shift Registers - led column control
    MOSFET - led layer control
    6 buttons - control the snake

    SOURCES
    ShiftPWM Library by Elco Jacobs http://www.elcojacobs.com/shiftpwm/

*/

#include "Snake.h"

// Pin 8 for shift register latch
// Data & Clock pin are define in the shiftpwn library using the SPI pins
const int ShiftPWM_latchPin = 8;

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
unsigned int numOutputs = numRegisters * 8;
unsigned int numRGBLeds = numRegisters * 8 / 3;
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

// Random point for snake to eat
int randomPoint[2] = { -1, -1}; // column, level

Snake snake;

void setup() {
  pinMode(7, OUTPUT);
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
  ShiftPWM.Start(pwmFrequency, maxBrightness);

  // Initialize snake object instance
  snake = Snake();
}

/*
   Button Press Functions
*/
void startGameIfNotStarted() {
  if (!gameStarted) {
    gameStarted = true;
  }
}

void upPressed() {
  startGameIfNotStarted();
  snake.changeDirection(2);
}

void rightPressed() {
  startGameIfNotStarted();
  snake.changeDirection(5);
}

void downPressed() {
  startGameIfNotStarted();
  snake.changeDirection(4);
}

void leftPressed() {
  startGameIfNotStarted();
  snake.changeDirection(3);
}

void forwardPressed() {
  startGameIfNotStarted();
  snake.changeDirection(0);
}

void backPressed() {
  startGameIfNotStarted();
  snake.changeDirection(1);
}


// Print score of game
void gameOver(int score = 0) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(layerPins[i], LOW);
  }
  digitalWrite(layerPins[3], HIGH);

  switch (score) {
    case 1:
      ShiftPWM.SetOne(46, maxBrightness);
      for (int i = 40; i >= 32; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(24, maxBrightness);
      break;
    case 2:
      ShiftPWM.SetOne(47, maxBrightness);
      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      for (int i = 23; i >= 20; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(28, maxBrightness);
      ShiftPWM.SetOne(36, maxBrightness);
      for (int i = 44; i >= 40; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(32, maxBrightness);
      ShiftPWM.SetOne(24, maxBrightness);
      ShiftPWM.SetOne(16, maxBrightness);
      break;
    case 3:
      ShiftPWM.SetOne(47, maxBrightness);
      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      for (int i = 23; i >= 16; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(28, maxBrightness);
      ShiftPWM.SetOne(36, maxBrightness);
      ShiftPWM.SetOne(44, maxBrightness);
      ShiftPWM.SetOne(40, maxBrightness);
      ShiftPWM.SetOne(32, maxBrightness);
      ShiftPWM.SetOne(24, maxBrightness);
      break;
    case 4:
      for (int i = 55; i >= 52; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(44, maxBrightness);
      ShiftPWM.SetOne(36, maxBrightness);
      ShiftPWM.SetOne(28, maxBrightness);
      ShiftPWM.SetOne(20, maxBrightness);
      for (int i = 15; i >= 8; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      break;
    case 5:
      ShiftPWM.SetOne(15, maxBrightness);
      ShiftPWM.SetOne(23, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(47, maxBrightness);
      for (int i = 55; i >= 52; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(44, maxBrightness);
      ShiftPWM.SetOne(36, maxBrightness);
      ShiftPWM.SetOne(28, maxBrightness);
      ShiftPWM.SetOne(20, maxBrightness);
      for (int i = 12; i >= 8; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(16, maxBrightness);
      ShiftPWM.SetOne(24, maxBrightness);
      ShiftPWM.SetOne(32, maxBrightness);
      ShiftPWM.SetOne(40, maxBrightness);
      ShiftPWM.SetOne(48, maxBrightness);
      break;
    case 6:
      ShiftPWM.SetOne(47, maxBrightness);
      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      ShiftPWM.SetOne(23, maxBrightness);
      ShiftPWM.SetOne(15, maxBrightness);
      for (int i = 55; i >= 48; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(44, maxBrightness);
      ShiftPWM.SetOne(36, maxBrightness);
      ShiftPWM.SetOne(28, maxBrightness);
      ShiftPWM.SetOne(20, maxBrightness);
      for (int i = 12; i >= 8; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(16, maxBrightness);
      ShiftPWM.SetOne(24, maxBrightness);
      ShiftPWM.SetOne(32, maxBrightness);
      ShiftPWM.SetOne(40, maxBrightness);
      break;
    case 7:
      ShiftPWM.SetOne(55, maxBrightness);
      ShiftPWM.SetOne(47, maxBrightness);
      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      ShiftPWM.SetOne(23, maxBrightness);
      for (int i = 15; i >= 8; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      break;
    case 8:
      for (int i = 55; i >= 48; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(47, maxBrightness);
      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      ShiftPWM.SetOne(23, maxBrightness);
      ShiftPWM.SetOne(44, maxBrightness);
      ShiftPWM.SetOne(36, maxBrightness);
      ShiftPWM.SetOne(28, maxBrightness);
      ShiftPWM.SetOne(20, maxBrightness);
      for (int i = 15; i >= 8; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(40, maxBrightness);
      ShiftPWM.SetOne(32, maxBrightness);
      ShiftPWM.SetOne(24, maxBrightness);
      ShiftPWM.SetOne(16, maxBrightness);
      break;
    case 9:
      for (int i = 55; i >= 52; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(47, maxBrightness);
      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      ShiftPWM.SetOne(23, maxBrightness);
      ShiftPWM.SetOne(44, maxBrightness);
      ShiftPWM.SetOne(36, maxBrightness);
      ShiftPWM.SetOne(28, maxBrightness);
      ShiftPWM.SetOne(20, maxBrightness);
      for (int i = 15; i >= 8; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      break;
    case 10:
      for (int i = 63; i >= 56; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      for (int i = 47; i >= 40; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }

      ShiftPWM.SetOne(39, maxBrightness);
      ShiftPWM.SetOne(31, maxBrightness);
      ShiftPWM.SetOne(23, maxBrightness);
      ShiftPWM.SetOne(15, maxBrightness);
      for (int i = 7; i >= 0; i--) {
        ShiftPWM.SetOne(i, maxBrightness);
      }
      ShiftPWM.SetOne(32, maxBrightness);
      ShiftPWM.SetOne(24, maxBrightness);
      ShiftPWM.SetOne(16, maxBrightness);
      ShiftPWM.SetOne(8, maxBrightness);
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
  snake.reset();
  randomPoint[0] = -1;
  randomPoint[1] = -1;
  gameStarted = false;
}


// Get random point for snake to eat
void getRandomPoint() {
  bool overLap = false;
  do {
    randomPoint[0] = random(0, 63);
    randomPoint[1] = random(0, 3);
    overLap = snake.pointOverLap(randomPoint[0], randomPoint[1]);
  } while(overLap == false);
  snake.setFood(randomPoint[0], randomPoint[1]);
}

// boolean to check if the arduino was just turned
int firstRun = true;

void loop() {
  // Cool startup animationg, turning on each layer 1 by 1 and speeding up
  int delayTime = 300;
  while (firstRun) {
    for (int i = 0; i < 10; i++) {
      for (int x = 0; x < 4; x++) {
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
  while (gameStarted) {
    // Retrieve a random point for snake to eat
    if (randomPoint[0] == -1 || randomPoint[1] == -1) {
      getRandomPoint();
    }

    // Move snake if not gameover
    if(!snake.shift()) {
      gameOver(snake.getLength());
    }

    // Loop through so that LEDs are on for certain amount of time according to snake speed
    for (int j = 0; j < snake.getSpeed(); j++) {
      // Loop through cube layers
      for (int x = 0; x < 4; x++) {
        digitalWrite(layerPins[x], HIGH);
        // Loop through snake and light LEDs
        for (int i = 0; i < snake.getLength(); i++) {
          if (randomPoint[1] == x) {
            ShiftPWM.SetOne(randomPoint[0], maxBrightness); // Light random snake food
          }
          if (snake.levelAt(i) == x) {
            ShiftPWM.SetOne(snake.columnAt(i), maxBrightness); // Light Snake
          }
        }
        ShiftPWM.SetAll(0); // Turn off shift registers
        digitalWrite(layerPins[x], LOW); // Turn off layer
      }
    }
  }
}
